# kwin-cursor-wrap

Horizontal monitor edge cursor wrapping for **KDE Plasma 6 (Wayland)**, supporting mixed resolutions, physical sizes, and fractional scaling.

## How It Works
- **KWin Script (`cursor-wrap`):** Monitors `workspace.cursorPosChanged` across outer left/right screen boundaries and maps the vertical coordinate proportionally between screens.
- **User Daemon (`cursor_warper`):** Receives `(dx, dy)` deltas over D-Bus (`org.berrybyte.CursorWarper`) and injects relative pointer movement via `/dev/uinput`.

## Installation (Arch Linux)
> **Note:** This package is not yet published on the AUR due to ongoing AUR account registration closures, though publishing it there is planned once registrations reopen. For now, install directly from this repository using `makepkg`.

### 1. Build & Install
```bash
git clone [https://github.com/Byte-Entropy/kwin-cursor-wrap.git](https://github.com/Byte-Entropy/kwin-cursor-wrap.git)
cd kwin-cursor-wrap
makepkg -sric && rm -f *.pkg.tar.*
```

### 2. Permissions & Service Setup
Add your user to the `input` group so `cursor_warper` can access `/dev/uinput`:
```bash
sudo usermod -aG input $USER
```

Then reload systemd, enable/start the user daemon, and reload KWin:
```bash
systemctl --user daemon-reload
systemctl --user enable --now cursor-warper.service
qdbus6 org.kde.KWin /KWin reconfigure
```
*(If you were not previously in the `input` group, log out and back in once for group permissions to take effect.)*
