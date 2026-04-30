# Display Rotation

Supported devices can rotate the display for different mounting orientations (for example to change which side the power cable exits from). Set `display_rotation` to rotate both the screen and touch input.

::: warning
ESPHome 2026.4 and newer handle rotation through LVGL (`lvgl.rotation`), including touch input. Older examples that set `touch_swap_xy`, `touch_mirror_x`, or `touch_mirror_y` are no longer needed.
:::

## ESP32-S3 4848S040

The 480×480 square display supports all four rotations.

| `display_rotation` |
| ------------------- |
| `"0"` (default)     |
| `"90"`              |
| `"180"`             |
| `"270"`             |

### Example: 90-degree rotation

```yaml
substitutions:
  name: "music-dashboard"
  friendly_name: "Music Dashboard"
  display_rotation: "90"

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

packages:
  music_dashboard:
    url: https://github.com/jtenniswood/esphome-media-player
    files: [devices/guition-esp32-s3-4848s040/packages.yaml]
    ref: main
    refresh: 1s
```

### Example: 270-degree rotation

```yaml
substitutions:
  name: "music-dashboard"
  friendly_name: "Music Dashboard"
  display_rotation: "270"

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

packages:
  music_dashboard:
    url: https://github.com/jtenniswood/esphome-media-player
    files: [devices/guition-esp32-s3-4848s040/packages.yaml]
    ref: main
    refresh: 1s
```

## ESP32-P4 86 Panel

The 720x720 square display supports the same four rotations as the S3 square display.

| `display_rotation` |
|---|
| `"0"` |
| `"90"` |
| `"180"` |
| `"270"` |

Example:

```yaml
substitutions:
  display_rotation: "90"

packages:
  music_dashboard:
    url: https://github.com/jtenniswood/esphome-media-player
    files: [devices/esp32-p4-86-panel/packages.yaml]
    ref: main
    refresh: 1s
```

## ESP32-P4 JC8012P4A1

The rectangular display defaults to landscape orientation and supports all four rotations. The layout changes automatically: landscape uses artwork on the left with track details on the right, while portrait uses artwork at the top with track details underneath.

| `display_rotation` |
| ------------------- |
| `"0"`               |
| `"90"` (default)    |
| `"180"`             |
| `"270"`             |

The device also exposes **Screen Rotation** in Home Assistant as a dropdown with `0`, `90`, `180`, and `270` options.

### Example: portrait rotation

```yaml
substitutions:
  name: "music-dashboard-10inch"
  friendly_name: "Music Dashboard 10inch"
  display_rotation: "0"

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

packages:
  music_dashboard:
    url: https://github.com/jtenniswood/esphome-media-player
    files: [devices/guition-esp32-p4-jc8012p4a1/packages.yaml]
    ref: main
    refresh: 1s
```

## ESP32-P4 JC4880P443

The 480x800 portrait display defaults to portrait orientation and supports all four rotations. Use `"180"` if you want the portrait layout flipped upside down. For landscape mounts, use the dedicated layout preset below. It still uses ESPHome 2026.4's native LVGL rotation; the preset just changes the layout values to 800x480 so the portrait UI is not clipped.

| `display_rotation` |
| ------------------- |
| `"0"` (default)     |
| `"90"`              |
| `"180"`             |
| `"270"`             |

The device also exposes **Screen Rotation** in Home Assistant as a dropdown with `0`, `90`, `180`, and `270` options.

### Example: 180-degree rotation

```yaml
substitutions:
  name: "music-dashboard-43inch"
  friendly_name: "Music Dashboard 4.3inch"
  display_rotation: "180"

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

packages:
  music_dashboard:
    url: https://github.com/jtenniswood/esphome-media-player
    files: [devices/guition-esp32-p4-jc4880p443/packages.yaml]
    ref: main
    refresh: 1s
```

### Example: landscape rotation

```yaml
substitutions:
  name: "music-dashboard-43inch"
  friendly_name: "Music Dashboard 4.3inch"
  display_rotation: "90"

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

packages:
  music_dashboard:
    url: https://github.com/jtenniswood/esphome-media-player
    files: [devices/guition-esp32-p4-jc4880p443/packages-90.yaml]
    ref: main
    refresh: 1s
```

Use `display_rotation: "270"` with the same `packages-90.yaml` preset if you want the landscape layout flipped the other way.
