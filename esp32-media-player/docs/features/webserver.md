---
title: Web Settings for ESPHome Media Player
description: Configure ESPHome Media Player from the device web settings page, including Home Assistant media player selection, brightness, screen saver, and updates.
---

# Web Settings

The firmware includes a built-in local-only web settings page for basic setup and advanced customization. After the device is on your Wi-Fi network, open the device IP address in a browser on the same network to choose what it controls, tune the display, and adjust behaviour without editing YAML or reflashing.

You can also open the same page from Home Assistant by going to **Settings -> Devices & Services -> ESPHome**, selecting the media player device, and clicking **Visit**.

## What it is for

The webserver is the main setup page for current firmware. It lets you choose the Home Assistant media player to control, tune the screen behaviour, and manage firmware updates from your browser.

The page is split across two tabs:

### Settings tab

Use this tab for the main setup and day-to-day display behaviour:

- **Media Player** — choose the main Home Assistant `media_player` entity the touchscreen controls.
- **Advanced** — set an optional linked media player for TV or Line-in sources.
- **Playback** — change track time display, progress bar visibility, and supported track info timing.
- **Volume** — control whether the speaker panel closes automatically after a short delay.
- **Idle Screen** — dim the display after playback pauses.
- **Screen Saver** — choose what happens after the device has been idle for longer.
- **Night Schedule** — set overnight screen-off behaviour and temporary wake timing.

### Device tab

Use this tab for device-wide options and maintenance:

- **Clock** — choose the on-screen clock format and timezone used by schedules.
- **Day/Night** — optionally provide a Home Assistant entity to control day and night mode.
- **Screen Brightness** — set active daytime and night-time brightness.
- **Screen Tone** — adjust warm tinting for album art on ESP32-P4 devices.
- **Rotation** — change screen rotation on supported ESP32-P4 panels.
- **Firmware** — check for updates, install an available update, and change automatic update behaviour.

See [Settings](/features/settings) for the full list of controls.

## Opening the page

1. Make sure your computer or phone is connected to the same network as the device.
2. Find the device IP address in your router, ESPHome, or Home Assistant.
3. Open `http://DEVICE-IP` in your browser, replacing `DEVICE-IP` with the real address.

If Home Assistant has discovered the ESPHome device, you can use **Visit** from the ESPHome device page instead of typing the IP address manually.

## First-time setup

On a fresh install, open the webserver and set **Media Player** to the Home Assistant entity you want the touchscreen to control, for example `media_player.living_room_speaker`.

When this value is saved, the device may reboot so the firmware can subscribe to the correct Home Assistant entity. This is expected. After it comes back online, the touchscreen should show artwork and controls for the selected media player.

## How the page loads

The device serves the settings page on port `80`. The firmware uses the project's hosted web UI bundle, so the device needs internet access the first time your browser loads the page.

The browser talks directly to the device while you make changes.

::: info 4" ESP32-S3 displays
Opening the webserver shows a **Web settings active** screen on the device while the browser is connected. This conserves available memory while the settings page is in use. Closing the browser tab or otherwise ending the connection restores the normal media player screen.
:::

## Troubleshooting

If the page does not open:

- Check that the device and browser are on the same network.
- Try the Home Assistant **Visit** button if you are unsure of the IP address.
- Use `http://` rather than `https://`.
- Wait for the device to finish booting after a firmware update or settings save.

If the page opens but settings do not apply, check that Home Assistant can see the device and that the selected entity ID is valid. For wider connection problems, see [Troubleshooting](/advanced/troubleshooting).
