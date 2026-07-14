#!/usr/bin/env bash
# install-ncurses-linux.sh
# Installs ncurses development libraries for Linux (Debian/Ubuntu, Fedora/RHEL, Arch)
# Usage: ./scripts/install-ncurses-linux.sh

set -euo pipefail

echo "=== Installing ncurses for Linux ==="
echo

# Detect package manager
if command -v apt-get &> /dev/null; then
    echo "Detected: Debian/Ubuntu (apt)"
    sudo apt-get update
    sudo apt-get install -y libncurses-dev
    echo "Installed: libncurses-dev"

elif command -v dnf &> /dev/null; then
    echo "Detected: Fedora/RHEL (dnf)"
    sudo dnf install -y ncurses-devel
    echo "Installed: ncurses-devel"

elif command -v dnf &> /dev/null; then
    echo "Detected: Fedora (dnf)"
    sudo dnf install -y ncurses-devel
    echo "Installed: ncurses-devel"

elif command -v pacman &> /dev/null; then
    echo "Detected: Arch Linux (pacman)"
    sudo pacman -S --needed ncurses
    echo "Installed: ncurses"

elif command -v yum &> /dev/null; then
    echo "Detected: RHEL/CentOS (yum)"
    sudo yum install -y ncurses-devel
    echo "Installed: ncurses-devel"

elif command -v zypper &> /dev/null; then
    echo "Detected: openSUSE (zypper)"
    sudo zypper install -y ncurses-devel
    echo "Installed: ncurses-devel"

else
    echo "Error: No supported package manager found."
    echo "Please install ncurses-devel manually:"
    echo "  Debian/Ubuntu: sudo apt install libncurses-dev"
    echo "  Fedora/RHEL:   sudo dnf install ncurses-devel"
    echo "  Arch Linux:    sudo pacman -S ncurses"
    exit 1
fi

echo
echo "Verifying installation..."
if pkg-config --exists ncurses 2>/dev/null; then
    echo "✓ ncurses found via pkg-config"
elif [ -f /usr/include/ncurses.h ]; then
    echo "✓ ncurses.h found in /usr/include"
elif [ -f /usr/include/ncurses/ncurses.h ]; then
    echo "✓ ncurses.h found in /usr/include/ncurses/"
else
    echo "Warning: Could not verify installation. ncurses may not be in standard paths."
    exit 1
fi

echo
echo "=== Installation complete ==="
echo "Compile with: g++ -o your_editor main.cpp -lncurses"
