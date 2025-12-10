# TypeNoodle

A modern, cross-platform desktop font manager built with Qt 6 (QML + C++) and CMake.

![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows%20%7C%20macOS-blue)
![Qt](https://img.shields.io/badge/Qt-6.6+-green)
![C++](https://img.shields.io/badge/C%2B%2B-20-orange)

## Features

- 🔍 **Font Discovery** - Automatically scans and indexes system fonts
- 👁️ **Live Preview** - Real-time font preview with customizable text and size
- 📁 **Collections** - Organize fonts into custom collections
- 🎨 **Dark Theme** - Beautiful Catppuccin Mocha color scheme
- ⚡ **Fast** - Native performance with Qt and C++20
- 🖥️ **Cross-Platform** - Linux, Windows, and macOS support

## Screenshots

*Coming soon*

## Build Requirements

### All Platforms

- CMake 3.21 or later
- C++20 compatible compiler
- Qt 6.6 or later (with Quick, QuickControls2, Widgets, Gui, Core)

### Platform-Specific

**Linux:**
- GCC 10+ or Clang 12+
- X11 development libraries

**Windows:**
- Visual Studio 2019 or later (MSVC)

**macOS:**
- Xcode 13 or later

## Building from Source

### 1. Clone the Repository

```bash
git clone --recursive https://github.com/yourusername/TypeNoodle.git
cd TypeNoodle
```

If you already cloned without `--recursive`, initialize submodules:

```bash
git submodule update --init --recursive
```

### 2. Install Qt 6

Download and install Qt 6.6+ from [qt.io](https://www.qt.io/download) or use your package manager:

**Linux (Ubuntu/Debian):**
```bash
sudo apt install qt6-base-dev qt6-declarative-dev qt6-quickcontrols2-dev
```

**macOS (Homebrew):**
```bash
brew install qt@6
```

**Windows:**
Download the Qt installer from [qt.io](https://www.qt.io/download)

### 3. Configure and Build

**Linux/macOS:**
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

**Windows (Visual Studio):**
```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

### 4. Run

**Linux:**
```bash
./typenoodle
```

**Windows:**
```cmd
Release\typenoodle.exe
```

**macOS:**
```bash
open typenoodle.app
```

## Project Structure

```
TypeNoodle/
├── CMakeLists.txt           # Main build configuration
├── .github/workflows/       # CI/CD workflows
├── external/                # Git submodules
│   ├── spdlog/             # Logging library
│   ├── json/               # JSON for C++
│   └── freetype/           # Font parsing (optional)
├── src/                    # C++ source code
│   ├── main.cpp
│   ├── core/              # Core font management
│   ├── models/            # Qt models for QML
│   └── utils/             # Utilities (config, logging)
├── qml/                   # QML UI files
│   ├── Main.qml
│   ├── Theme.qml
│   ├── components/
│   └── pages/
└── resources/            # Icons, fonts, etc.
```

## Configuration

TypeNoodle stores its configuration in platform-specific locations:

- **Linux:** `~/.config/TypeNoodle/`
- **Windows:** `%APPDATA%/TypeNoodle/`
- **macOS:** `~/Library/Application Support/TypeNoodle/`

### Configuration Files

- `config.json` - Application settings
- `collections.json` - User font collections
- `typenoodle.log` - Application logs

## Development

### Code Style

- **C++:** Modern C++20, follow Google C++ Style Guide
- **QML:** Keep logic minimal, delegate to C++ backend
- **Naming:** PascalCase for types, camelCase for properties/functions

### Dependencies

- **spdlog** - Fast C++ logging library
- **nlohmann/json** - JSON for Modern C++
- **FreeType** (optional) - Advanced font parsing

### Adding Features

1. Create a new branch: `git checkout -b feature/your-feature`
2. Make your changes
3. Test on all target platforms
4. Submit a pull request

## CI/CD

TypeNoodle uses GitHub Actions for continuous integration:

- Builds on Linux, Windows, and macOS
- Runs on every push and pull request
- Artifacts available for download after each build

## License

[Add your license here]

## Contributing

Contributions are welcome! Please read the contributing guidelines before submitting a pull request.

## Acknowledgments

- [Qt](https://www.qt.io/) - Cross-platform application framework
- [spdlog](https://github.com/gabime/spdlog) - Fast C++ logging
- [nlohmann/json](https://github.com/nlohmann/json) - JSON for Modern C++
- [Catppuccin](https://github.com/catppuccin/catppuccin) - Soothing pastel theme

## Support

For bugs and feature requests, please [open an issue](https://github.com/yourusername/TypeNoodle/issues).
