# NotepadNext Plus

A fork of [NotepadNext](https://github.com/dail8859/NotepadNext) with improved typography and editing features for macOS.

---

## What's Improved

### Before vs After

| Original NotepadNext | NotepadNext Plus |
|---------------------|------------------|
| ![Before](docs/before.png) | ![After](docs/after.png) |
| Cramped lines, Courier New font | Menlo font, comfortable line spacing |

---

## New Features

### 1. Better Typography

| Feature | Before | After |
|---------|--------|-------|
| **Font** | Courier New | Menlo (native macOS) |
| **Font Size** | System default | +2 points larger |
| **Line Spacing** | None | 2px above & below each line |

### 2. Strip Trailing Whitespace

Removes invisible spaces and tabs at the end of lines.

**Problem solved**: When you press backspace on an "empty" line and nothing happens - that's because there are hidden spaces. This feature removes them.

**Menu**: Edit → Line Operations → Strip Trailing Whitespace

![Strip Menu](docs/strip-menu.png)

### 3. Auto-Strip on Save

Automatically cleans trailing whitespace every time you save.

**Location**: Preferences → Editor → Strip trailing whitespace on save

![Preferences](docs/preferences.png)

### 4. Adjustable Line Spacing

Control the space between lines in Preferences.

**Location**: Preferences → Editor → Line Spacing

---

## Installation

### Build from Source (macOS)

```bash
# Install dependencies
brew install cmake qt@6

# Clone
git clone https://github.com/Basheer-Raad/NotepadNext-Plus-.git
cd NotepadNext-Plus-

# Build
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)
make -j$(sysctl -n hw.ncpu)

# Run
open src/NotepadNext.app
```

---

## Summary of Changes

| Feature | Description |
|---------|-------------|
| Default font → Menlo | Better rendering on macOS |
| Line spacing (+2px) | More readable, less cramped |
| Strip Trailing Whitespace menu | Clean hidden spaces manually |
| Auto-strip on save | Clean hidden spaces automatically |
| Line Spacing preference | Customize spacing in settings |

---

## Credits

- Original [NotepadNext](https://github.com/dail8859/NotepadNext) by Justin Dailey
- Typography & whitespace improvements by Basheer Raad

---

## License

[GNU General Public License version 3](https://www.gnu.org/licenses/gpl-3.0.txt)
