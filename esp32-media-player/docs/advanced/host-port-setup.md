---
title: Home Assistant Host and Port Setup
description: Configure Home Assistant protocol, host, port, and SSL options so ESPHome Media Player can load album artwork correctly.
---

# Host/Port Setup

By default the device constructs artwork URLs using `http://homeassistant.local:8123`. If your Home Assistant instance uses HTTPS, a different hostname/IP, or a non-standard port, album art will fail to load.

To fix this, add the relevant substitutions to your configuration:

```yaml
substitutions:
  name: "music-dashboard"
  friendly_name: "Music Dashboard"
  ha_protocol: "https"
  ha_host: "192.168.1.100"
  ha_port: "8123"
  ha_verify_ssl: "false"  # set to "false" for self-signed or local CA certs
```

The device builds artwork URLs as `<ha_protocol>://<ha_host>:<ha_port>/api/...`, so the protocol, host, and port must all match whatever Home Assistant is reachable on from the device's network.

## HTTPS (SSL) configuration

If Home Assistant is served over HTTPS (for example via the [Let's Encrypt add-on](https://www.home-assistant.io/common-tasks/ssl_tls/) with SSL on port 8123, or behind a reverse proxy), artwork requests will fail unless you tell the device to use HTTPS.

1. **Set the protocol** — Add `ha_protocol: "https"` so artwork URLs use `https://` instead of `http://`.

2. **Use a hostname for HTTPS** — For HTTPS the device uses the configured `ha_host` (it does not switch to the API client IP). The hostname must match the hostname on your SSL certificate (e.g. `homeassistant.local` or your FQDN). Using a raw IP with HTTPS will cause certificate mismatch errors.

3. **Self-signed or local CA certificates** — If your certificate is self-signed or signed by a local CA that the device doesn't trust, set `ha_verify_ssl: "false"` so the HTTP client skips certificate verification. Only do this on a trusted local network.

**Example: HTTPS with Let's Encrypt (trusted cert)**

```yaml
substitutions:
  name: "music-dashboard"
  friendly_name: "Music Dashboard"
  ha_protocol: "https"
  ha_host: "homeassistant.local"   # or your FQDN — must match cert
  ha_port: "8123"
  # ha_verify_ssl defaults to "true"; no need to set it for public CAs
```

**Example: HTTPS with self-signed or local CA cert**

```yaml
substitutions:
  name: "music-dashboard"
  friendly_name: "Music Dashboard"
  ha_protocol: "https"
  ha_host: "homeassistant.local"
  ha_port: "8123"
  ha_verify_ssl: "false"   # required for self-signed or local CA
```

Default behaviour remains `http` on port `8123` with SSL verification enabled, so existing setups are unchanged.

## Local artwork with self-signed certificates

Some media players, including Wiim devices, publish album artwork as a direct `https://` URL on the local network. If that local artwork server uses a self-signed certificate, the display cannot verify it and the artwork download will fail.

For that case, leave the normal Home Assistant SSL settings as they are and enable local artwork bypass only:

```yaml
substitutions:
  allow_insecure_local_artwork: "true"
```

This only applies to local/private HTTPS artwork URLs, such as `192.168.x.x`, `10.x.x.x`, `172.16-31.x.x`, link-local addresses, `localhost`, and `.local` hostnames. Public internet HTTPS artwork continues to use certificate verification. Only enable this on a trusted local network.
