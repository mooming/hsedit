# Setup Scripts

One-click installation scripts for ncurses on each platform.

## Quick Start

```bash
# Linux
chmod +x install-ncurses-linux.sh
./scripts/install-ncurses-linux.sh

# macOS
chmod +x install-ncurses-macos.sh
./scripts/install-ncurses-macos.sh

# Windows
scripts\install-ncurses-windows.bat
```

## Compile Your Editor

```bash
# Linux / macOS (system ncurses)
g++ -o hsedit main.cpp -lncurses

# macOS (Homebrew ncurses)
g++ -o hsedit main.cpp -I$(brew --prefix)/include -L$(brew --prefix)/lib -lncurses

# Windows (MSYS2)
g++ -o hsedit.exe main.cpp -lncurses
```

## What These Scripts Do

| Script | Platform | What It Installs |
|--------|----------|-----------------|
| `install-ncurses-linux.sh` | Debian/Ubuntu, Fedora/RHEL, Arch, openSUSE | `libncurses-dev` or `ncurses-devel` |
| `install-ncurses-macos.sh` | macOS (via Homebrew) | `ncurses` from Homebrew |
| `install-ncurses-windows.bat` | Windows (via MSYS2) | `mingw-w64-ncurses` |

## Verifying Installation

After running any script, verify ncurses is available:

```bash
# Check header exists
ls /usr/include/ncurses.h
ls /usr/include/ncurses/ncurses.h
ls C:\msys64\mingw64\include\ncurses.h

# Check library (Linux/macOS)
ldconfig -p | grep ncurses

# Compile a test
echo '#include <ncurses.h>
int main(){initscr();endwin();return 0;}' > test_ncurses.c
gcc test_ncurses.c -lncurses -o test_ncurses && echo "OK"
```
