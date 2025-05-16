# ASCII Raycaster

A minimal real-time 3D raycaster in pure C++ and the Win32 Console API.  
Render walls, floor, ceiling, and a toggleable (tap 'M') top-down debug map—all in ASCII.

---

## 🚀 Features

- **Real-time raycasting** with distance-based ASCII shading
- **Debug map overlay** showing player position & facing direction (tap 'M' to toggle)
- **Configurable** FOV, view depth, resolution
- **WASD controls** + A/D rotation
- **Single-file demo** (just drop `main.cpp` and build)

---

## 🛠 Requirements

- Windows 10+ (for Console API)
- Microsoft Visual C++ (MSVC) or any C++17-capable compiler
- CMake for building

-- for terminal options, feel free to change the font, but you will need to play around with width --
- Terminal size - 120 x 30 
- Font - Consolas
- Line Height - 1

---

## 🏗️ Build

```bash
git clone 
cd AsciiRayCastFPS
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## 🙏 Credits
 [javidx9 - Terminal Based Raycaster](https://youtu.be/xW8skO7MFYw)

 Great tutorial on raycasting in C++ which I used as a base for this project and then tweaked to my liking.

## 🎯 Future-Goals
I would like to add a few features to this project in the future, such as:
- **Ascii Sprite Editor** - A GUI editor for creating ASCII-based sprites and maps.
- **Simple Depth Buffer** - For more complex 3D rendering.
- **Game Specific Abstractions** - Turning this into a lightweight ASCII framework for creating games.
- **More Game Mechanics** - Adding enemies, items, and other game mechanics.