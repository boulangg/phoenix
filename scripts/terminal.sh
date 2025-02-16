#!/bin/sh
# 
# Source: https://github.com/i3/i3/blob/next/i3-sensible-terminal
#
# This code is released in public domain by Han Boetes <han@mijncomputer.nl>
#
# This script tries to exec a terminal emulator by trying some known terminal
# emulators.
#
# We welcome patches that add distribution-specific mechanisms to find the
# preferred terminal emulator. On Debian, there is the x-terminal-emulator
# symlink for example.
for terminal in "$TERMINAL" x-terminal-emulator urxvt rxvt termit terminator Eterm aterm uxterm xterm gnome-terminal roxterm xfce4-terminal termite lxterminal mate-terminal terminology st qterminal lilyterm tilix terminix konsole; do
    if command -v "$terminal" > /dev/null 2>&1; then
        echo "$terminal $@"
        exec "$terminal" "$@"
    fi
done
