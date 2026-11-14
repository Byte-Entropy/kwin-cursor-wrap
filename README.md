# kwin-cursor-wrap

Horizontal monitor edge cursor wrapping for **KDE Plasma 6 (Wayland)**, supporting mixed resolutions, physical sizes, and fractional scaling.

## How It Works
- **KWin Script (`cursor-wrap`):** Monitors `workspace.cursorPosChanged` across outer left/right screen boundaries and maps the vertical coordinate proportionally between screens.
- **User Daemon (`cursor_warper`):** Receives `(dx, dy)` deltas over D-Bus (`org.berrybyte.CursorWarper`) and injects relative pointer movement via `/dev/uinput`.

## Installation (Arch Linux / AUR)
```bash
yay -S kwin-cursor-wrap
sudo usermod -aG input $USER
```

Log out and back in (or run `systemctl --user enable --now cursor-warper.service && qdbus6 org.kde.KWin /KWin reconfigure` if already in the `input` group).
