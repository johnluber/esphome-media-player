#include "image_decoder.h"
#include "artwork_image.h"

#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace artwork_image {

static const char *const TAG = "artwork_image.decoder";

// Static shared buffer — one allocation reused by all DownloadBuffer instances.
// Only one artwork_image downloads at a time so this is safe.
uint8_t *DownloadBuffer::shared_buffer_ = nullptr;
size_t DownloadBuffer::shared_buffer_size_ = 0;

bool ImageDecoder::set_size(int width, int height) {
  bool success = this->image_->resize_(width, height) > 0;
  if (!success) {
    this->failed_ = true;
    return false;
  }
  int content_width = this->image_->decode_content_width_ > 0 ? this->image_->decode_content_width_
                                                              : this->image_->decode_buffer_width_;
  int content_height = this->image_->decode_content_height_ > 0 ? this->image_->decode_content_height_
                                                                : this->image_->decode_buffer_height_;
  this->x_offset_ = this->image_->decode_offset_x_;
  this->y_offset_ = this->image_->decode_offset_y_;
  this->x_scale_ = static_cast<double>(content_width) / width;
  this->y_scale_ = static_cast<double>(content_height) / height;
  ESP_LOGI(TAG, "Decoder geometry: source=%dx%d content=%dx%d offset=%d,%d scale=%.4f,%.4f",
           width, height, content_width, content_height, this->x_offset_, this->y_offset_, this->x_scale_,
           this->y_scale_);
  return success;
}

void ImageDecoder::draw(int x, int y, int w, int h, const Color &color) {
  if (this->failed_) {
    return;
  }
  auto width = std::min(this->image_->decode_buffer_width_,
                        this->x_offset_ + static_cast<int>(std::ceil((x + w) * this->x_scale_)));
  auto height = std::min(this->image_->decode_buffer_height_,
                         this->y_offset_ + static_cast<int>(std::ceil((y + h) * this->y_scale_)));
  for (int i = this->x_offset_ + static_cast<int>(x * this->x_scale_); i < width; i++) {
    for (int j = this->y_offset_ + static_cast<int>(y * this->y_scale_); j < height; j++) {
      this->image_->draw_pixel_(i, j, color);
    }
  }
}

void ImageDecoder::draw_rgb565_block(int x, int y, int w, int h, const uint8_t *data) {
  if (this->failed_) {
    return;
  }
  int bpp_bytes = this->image_->get_bpp() / 8;

  if (this->x_scale_ == 1.0 && this->y_scale_ == 1.0 && bpp_bytes == 2) {
    for (int row = 0; row < h; row++) {
      int dy = this->y_offset_ + y + row;
      if (dy < 0 || dy >= this->image_->decode_buffer_height_)
        continue;
      int start_x = std::max(0, this->x_offset_ + x);
      int end_x = std::min(this->x_offset_ + x + w, this->image_->decode_buffer_width_);
      if (start_x >= end_x)
        continue;
      int copy_w = end_x - start_x;
      int src_offset = (row * w + (start_x - this->x_offset_ - x)) * 2;
      int dst_pos = this->image_->get_position_(start_x, dy);
      memcpy(this->image_->decode_buffer_ + dst_pos, data + src_offset, copy_w * 2);
    }
    return;
  }

  for (int row = 0; row < h; row++) {
    for (int col = 0; col < w; col++) {
      int src_x = x + col;
      int src_y = y + row;
      int src_offset = (row * w + col) * 2;

      auto target_w = std::min(this->image_->decode_buffer_width_,
                               this->x_offset_ + static_cast<int>(std::ceil((src_x + 1) * this->x_scale_)));
      auto target_h = std::min(this->image_->decode_buffer_height_,
                               this->y_offset_ + static_cast<int>(std::ceil((src_y + 1) * this->y_scale_)));
      for (int dy = this->y_offset_ + static_cast<int>(src_y * this->y_scale_); dy < target_h; dy++) {
        for (int dx = this->x_offset_ + static_cast<int>(src_x * this->x_scale_); dx < target_w; dx++) {
          int dst_pos = this->image_->get_position_(dx, dy);
          memcpy(this->image_->decode_buffer_ + dst_pos, data + src_offset, 2);
          if (bpp_bytes > 2) {
            this->image_->decode_buffer_[dst_pos + 2] = 0xFF;
          }
        }
      }
    }
  }
}

DownloadBuffer::DownloadBuffer(size_t size) : size_(size), unread_(0) {
  // No per-instance allocation. The shared buffer is allocated lazily on first
  // data() call, growing to accommodate the largest size_ seen.
}

uint8_t *DownloadBuffer::data(size_t offset) {
  // Grow shared buffer if needed (lazy, happens after PSRAM is available).
  if (this->size_ > shared_buffer_size_) {
    uint8_t *new_buf = static_cast<uint8_t *>(heap_caps_malloc(this->size_, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    if (!new_buf) {
      // Fallback to internal RAM
      new_buf = static_cast<uint8_t *>(heap_caps_malloc(this->size_, MALLOC_CAP_8BIT));
    }
    if (!new_buf) {
      ESP_LOGE(TAG, "Failed to allocate shared download buffer of size %zu", this->size_);
      this->size_ = 0;
      return nullptr;
    }
    if (shared_buffer_) {
      if (unread_ > 0) memcpy(new_buf, shared_buffer_, unread_);
      heap_caps_free(shared_buffer_);
    }
    shared_buffer_ = new_buf;
    shared_buffer_size_ = this->size_;
    ESP_LOGI(TAG, "Shared download buffer allocated: %zu bytes in PSRAM", this->size_);
  }
  if (offset > this->size_) {
    ESP_LOGE(TAG, "Tried to access beyond download buffer bounds!!!");
    return shared_buffer_;
  }
  return shared_buffer_ + offset;
}

size_t DownloadBuffer::read(size_t len) {
  if (len > this->unread_) {
    ESP_LOGE(TAG, "Decoder consumed %zu bytes, but only %zu were buffered", len, this->unread_);
    len = this->unread_;
  }
  this->unread_ -= len;
  if (this->unread_ > 0) {
    memmove(this->data(), this->data(len), this->unread_);
  }
  return this->unread_;
}

size_t DownloadBuffer::resize(size_t size) {
  if (shared_buffer_size_ >= size) {
    this->size_ = size;
    return size;
  }
  // Grow shared buffer
  uint8_t *new_buf = static_cast<uint8_t *>(heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
  if (!new_buf) {
    new_buf = static_cast<uint8_t *>(heap_caps_malloc(size, MALLOC_CAP_8BIT));
  }
  if (new_buf) {
    if (shared_buffer_ && unread_ > 0) {
      memcpy(new_buf, shared_buffer_, unread_);
    }
    if (shared_buffer_) heap_caps_free(shared_buffer_);
    shared_buffer_ = new_buf;
    shared_buffer_size_ = size;
    this->size_ = size;
    return size;
  } else {
    ESP_LOGE(TAG, "Shared buffer resize to %zu bytes failed.", size);
    this->size_ = 0;
    this->reset();
    return 0;
  }
}

}  // namespace artwork_image
}  // namespace esphome
