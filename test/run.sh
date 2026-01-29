#!/usr/bin/env bash
set -euo pipefail

CUSTOM_DISPLAY=:99

xorg_run() { DISPLAY="$CUSTOM_DISPLAY" "$@"; }

Xephyr "$CUSTOM_DISPLAY" -no-host-grab -screen 800x600 -noreset &
XEPHYR_PID=$!

cleanup() { kill "$XEPHYR_PID" 2>/dev/null || true; }
trap cleanup EXIT

# wait for X to accept connections
for _ in {1..100}; do
  xorg_run xdpyinfo >/dev/null 2>&1 && break
  sleep 0.05
done

# set root background (this exits immediately by design)
xorg_run xsetroot -solid "#ff0000"

# optional: open a window so you can test non-root drawables too
# xorg_run xclock >/dev/null 2>&1 &
# CLOCK_PID=$!

xorg_run xwd -root -silent | hexdump -C

xorg_run npx ava test/test.ts

# kill "$CLOCK_PID" 2>/dev/null || true
