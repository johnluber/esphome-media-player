---
title: ESPHome Media Player Screen Saver
description: Configure ESPHome Media Player screen saver behavior, including idle dimming, clock mode, screen-off scheduling, and day/night brightness.
---

# Screen Saver

When playback is paused and paused dimming is enabled, the device uses a two-stage screen saver to reduce power consumption and prevent burn-in. All settings are configurable from the device's web settings page. You can open it by visiting the device IP address directly in your browser, or from the device in Home Assistant under **Settings → Devices & Services → ESPHome** by clicking **Visit**.

## How it works

1. **Dimming** — If **Dim when idle** is on, after **Dim After** elapses (default: 60 s), the screen dims to the day or night dimmed brightness level.
2. **Idle action** — If **Screen Saver: Enabled** is on, after **Screen Saver: Timer** elapses (default: 300 s), the selected day or evening screen saver runs:
   - **Clock** shows a large `HH:MM` clock.
   - **Screen Off** turns the display fully off.
   - **Disabled** leaves the display at the dimmed level.

If **Screen Saver: Enabled** is off, the screen saver options are hidden and the display stays at the dimmed level after the dimming stage.

Any touch or new media playback instantly returns the display to full active brightness.

## Clock screen saver

The optional clock screen saver displays the current time in large, thin digits on a black background. Choose **Clock** as the day or evening screen saver to use it.

Set **Clock Format** to choose 24-hour time or 12-hour time. The 12-hour clock does not show AM/PM, keeping the digits compact enough for the clock to keep drifting for burn-in prevention.

The clock position drifts subtly each minute across a small region to prevent burn-in. Brightness is controlled independently for day and evening clock screen savers (default: 35%).

## Night Schedule

**Night Schedule** can turn the display fully off outside a daily time window. For example, an **On Time** of 6:00 AM and an **Off Time** of 8:00 PM keeps the screen available during the day and turns it off overnight.

Touching the screen during the off period wakes it temporarily. **When Woken, Idle Time To Screen Off** controls how long it stays awake before returning to scheduled screen-off.

## Day/Night awareness

Brightness levels and screen saver behavior adapt automatically based on whether it is currently "day" or "night":

- **Daytime** — uses **Day: Active Brightness**, **Day: Dim Brightness**, and **Daytime Screen Saver**.
- **Evening** — uses **Night: Active Brightness**, **Night: Dim Brightness**, and **Evening Screen Saver**.

This lets you configure different behavior for day and night — for example, showing the clock during the day but turning the screen off at night. On the ESP32-P4, [Screen Tone](/features/settings#screen-tone) warmth also follows the same day/night detection, so album art can shift warmer at night.

By default, the device reads the `sun.sun` entity in Home Assistant (sun above horizon = day, below horizon = night). You can override this with any `binary_sensor` or `input_boolean` entity by setting **Day/Night Source** on the web settings page.

### Custom day/night sensor

To use your own logic for day vs night:

1. Create a `binary_sensor` or `input_boolean` helper in Home Assistant that reflects your preferred day/night state (`on` = day, `off` = night).
2. Open the settings page by visiting the device IP address directly in your browser, or go to the device in Home Assistant under **Settings → Devices & Services → ESPHome** and click **Visit**. Enter the entity ID in **Day/Night Source** — for example, `binary_sensor.daytime` or `input_boolean.is_daytime`.
3. The device will immediately start using the custom sensor instead of `sun.sun`.

To revert to the default sun-based behavior, clear **Day/Night Source**.

This is useful when you want day/night to depend on more than just the sun — for example, room lighting levels, party mode, or a time-based schedule.

## Settings

### Timers

| Setting | Description | Default |
|---------|-------------|---------|
| **Dim when idle** | Enables screen dimming when playback is paused. | On |
| **Dim After** | Time after playback pauses before the screen dims. | 60 s |
| **Screen Saver: Enabled** | Enables the clock/off screen saver step after dimming. | On |
| **Screen Saver: Timer** | Time after dimming before the screen saver activates. | 300 s |

### Clock

| Setting | Description | Default |
|---------|-------------|---------|
| **Daytime Screen Saver** | What happens after the idle timer during the day: Clock, Screen Off, or Disabled. | Clock |
| **Evening Screen Saver** | What happens after the idle timer in the evening: Clock, Screen Off, or Disabled. | Clock |
| **Day Clock Brightness** | Backlight level for the daytime clock screen saver. | 35% |
| **Evening Clock Brightness** | Backlight level for the evening clock screen saver. | 35% |
| **Clock Format** | Clock display format: 24-hour or 12-hour without AM/PM. | 24 Hour |
| **Timezone** | IANA-style timezone for the clock and scheduled screen-off controls. The browser shows the current GMT offset. | UTC |

### Night Schedule

| Setting | Description | Default |
|---------|-------------|---------|
| **Screen: Schedule Enabled** | Enable the daily screen-off schedule. | Off |
| **Screen: Schedule On Hour** | Hour when the screen becomes available again. | 6 |
| **Screen: Schedule Off Hour** | Hour when the screen turns off for the schedule. | 20 |
| **Screen: Schedule Wake Timeout** | Temporary wake duration during scheduled off hours. | 60 s |

### Brightness

| Setting | Description | Default |
|---------|-------------|---------|
| **Day: Active Brightness** | Screen brightness during active use (daytime). | 100% |
| **Night: Active Brightness** | Screen brightness during active use (nighttime). | 80% |
| **Day: Dim Brightness** | Screen brightness when dimmed (daytime). | 35% |
| **Night: Dim Brightness** | Screen brightness when dimmed (nighttime). | 25% |

### Idle actions

| Setting | Description | Default |
|---------|-------------|---------|
| **Daytime Screen Saver** | Daytime action after dimming and the idle timer. Options: Clock, Screen Off, or Disabled. | Clock |
| **Evening Screen Saver** | Evening action after dimming and the idle timer. Options: Clock, Screen Off, or Disabled. | Clock |
