# Swayler

A modern system-wide theme manager for Linux with Wayland/Sway support, built with C++ and libadwaita.

## Features

- 🎨 Change GTK themes, icon themes, and cursor themes
- 💾 Save and load theme presets
- 🪟 Modern libadwaita UI (Wayland-native)
- ⚡ Written in C++20 for performance
- 🎯 Sway/Wayland first-class support

## Requirements

### Build Dependencies
- C++20 compatible compiler (GCC 10+, Clang 12+)
- CMake 3.16+
- GTK4 development files
- libadwaita development files
- nlohmann_json

### Runtime Requirements
- GTK4
- libadwaita
- GTK themes and icon themes installed

## Building

### Install Dependencies (Arch Linux)
```bash
sudo pacman -S cmake gcc gtk4 libadwaita nlohmann-json
```

### Install Dependencies (Ubuntu/Debian)
```bash
sudo apt install cmake g++ libgtk-4-dev libadwaita-1-dev nlohmann-json3-dev
```

### Build
```bash
git clone <repository>
cd swayler
mkdir build
cd build
cmake ..
make
```

### Install
```bash
sudo make install
```

Or run directly:
```bash
./bin/swayler
```

## Project Structure

```
swayler/
├── src/
│   ├── main.cpp              # Application entry point
│   ├── application.hpp/cpp   # GTK application class
│   ├── window.hpp/cpp        # Main window UI
│   └── theme_manager.hpp/cpp # Theme management logic
├── include/                  # Additional headers
├── resources/
│   ├── styles/custom.css     # Application styling
│   └── ui/                   # UI definitions
├── data/                     # Desktop files, schemas
├── CMakeLists.txt
└── README.md
```

## Usage

### Launch Application
```bash
swayler
```

### Select and Apply Themes
1. Choose GTK theme, icon theme, and cursor theme from dropdowns
2. Click "Apply Theme" to activate immediately
3. Changes apply system-wide

### Save Presets
1. Configure your desired theme combination
2. Click "Save Preset" and give it a name (e.g., "Gruvbox Dark")
3. Presets are saved to `~/.config/swayler/presets/`

### Load Presets
1. Select a preset from the list
2. Click "Load" to apply that preset

### Delete Presets
1. Select a preset from the list
2. Click "Delete" to remove it

## Configuration

Themes are stored in:
- `~/.config/gtk-3.0/settings.ini` - GTK3 theme config
- `~/.config/gtk-4.0/settings.ini` - GTK4 theme config
- `~/.config/swayler/presets/` - Saved theme presets

## Contributing

Contributions welcome! Areas for enhancement:
- Sway config integration (wallpaper, colors)
- Qt theme support
- Terminal color scheme integration
- Theme preview
- More sophisticated preset management

## License

MIT