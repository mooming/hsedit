#!/usr/bin/env bash
# install-ncurses-macos.sh
# Installs ncurses for macOS via Homebrew
# macOS includes a basic ncurses with Xcode, but Homebrew provides a newer version.
# Usage: ./scripts/install-ncurses-macos.sh

set -euo pipefail

echo "=== Installing ncurses for macOS ==="
echo

# Check for Homebrew
if ! command -v brew &> /dev/null; then
    echo "Homebrew not found. Installing Homebrew..."
    echo "  See: https://brew.sh"
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
fi

echo
echo "Installing ncurses via Homebrew..."
brew install ncurses

echo
echo "Verifying installation..."
if [ -f "$(brew --prefix)/include/ncurses.h" ]; then
    echo "✓ ncurses.h found at $(brew --prefix)/include/ncurses.h"
    NCURSES_PREFIX="$(brew --prefix)"
else
    echo "Warning: ncurses.h not found in Homebrew prefix."
    exit 1
fi

echo
echo "=== Installation complete ==="
echo "Compile with: g++ -o your_editor main.cpp -I${NCURSES_PREFIX}/include -L${NCURSES_PREFIX}/lib -lncurses"
