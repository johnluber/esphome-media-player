#pragma once
#include "esphome/core/color.h"

namespace esphome {
namespace artwork_image {

enum DecodeError : int {
  DECODE_ERROR_INVALID_TYPE = -1,
  DECODE_ERROR_UNSUPPORTED_FORMAT = -2,
  DECODE_ERROR_OUT_OF_MEMORY = -3,
};

class ArtworkImage;

/**
 * @brief Class to abstract decoding different image formats.
 */
class ImageDecoder {
 public:
  /**
   * @brief Construct a new Image Decoder object
   *
   * @param image The image to decode the stream into.
   */
  ImageDecoder(ArtworkImage *image) : image_(image) {}
  virtual ~ImageDecoder() = default;

  /**
   * @brief Initialize the decoder.
   *
   * @param download_size The total number of bytes that need to be downloaded for the image.
   * @return int          Returns 0 on success, a {@see DecodeError} value in case of an error.
   */
  virtual int prepare(size_t download_size) {
    this->download_size_ = download_size;
    return 0;
  }

  /**
   * @brief Decode a part of the image. It will try reading from the buffer.
   * There is no guarantee that the whole available buffer will be read/decoded;
   * the method will return the amount of bytes actually decoded, so that the
   * unread content can be moved to the beginning.
   *
   * @param buffer The buffer to read from.
   * @param size   The maximum amount of bytes that can be read from the buffer.
   * @return int   The amount of bytes read. It can be 0 if the buffer does not have enough content to meaningfully
   *               decode anything, or negative in case of a decoding error.
   */
  virtual int decode(uint8_t *buffer, size_t size) = 0;

  /**
   * @brief Request the image to be resized once the actual dimensions are known.
   * Called by the callback functions, to be able to access the parent Image class.
   *
   * @param width The image's width.
   * @param height The image's height.
   * @return true if the image was resized, false otherwise.
   */
  bool set_size(int width, int height);

  void draw(int x, int y, int w, int h, const Color &color);
  void draw_rgb565_block(int x, int y, int w, int h, const uint8_t *data);

  bool is_finished() const { return this->download_size_ > 0 && this->decoded_bytes_ >= this->download_size_; }
  bool has_unknown_download_size() const { return this->download_size_ == 0; }
  void set_download_size(size_t download_size) { this->download_size_ = download_size; }

 protected:
  ArtworkImage *image_;
  bool failed_{false};

  double x_scale_{1};
  double y_scale_{1};
  int x_offset_{0};
  int y_offset_{0};

  size_t download_size_ = 1;
  size_t decoded_bytes_ = 0;
};

/**
 * @brief Download buffer with a SHARED underlying allocation.
 *
 * Only one artwork_image downloads at a time, so all instances share a single
 * heap allocation. This eliminates per-instance SRAM usage at boot, allowing
 * any number of artwork_image instances without SRAM exhaustion on the JC8012.
 *
 * The shared buffer is allocated lazily on first use (after PSRAM is available).
 * Each DownloadBuffer instance tracks its own logical size and unread bytes but
 * points into the same physical memory.
 */
class DownloadBuffer {
 public:
  DownloadBuffer(size_t size);

  virtual ~DownloadBuffer() {
    // Shared buffer — never deallocate in destructor
  }

  uint8_t *data(size_t offset = 0);

  uint8_t *append() { return this->data(this->unread_); }

  size_t unread() const { return this->unread_; }
  size_t size() const { return this->size_; }
  size_t free_capacity() const { return this->size_ - this->unread_; }

  size_t read(size_t len);
  size_t write(size_t len) {
    this->unread_ += len;
    return this->unread_;
  }

  void reset() { this->unread_ = 0; }

  size_t resize(size_t size);

 protected:
  size_t size_;
  size_t unread_{0};

  // Shared across ALL DownloadBuffer instances — one allocation for the largest
  // buffer_size seen, reused by each instance in turn.
  static uint8_t *shared_buffer_;
  static size_t shared_buffer_size_;
};

}  // namespace artwork_image
}  // namespace esphome
