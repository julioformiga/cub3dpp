# CUB3DPP - 3D Graphics with C++ & SFML

This repository contains a personal implementation and exploration of raycasting techniques to support learning for the CUB3D project found in School 42's core curriculum. The implementation draws inspiration from "The Hello World Guy"'s Raycasting tutorial series:
- YouTube Channel: [The Hello World Guy](https://www.youtube.com/@thehelloworldguyofficial)
- Tutorial Series: [Raycasting](https://www.youtube.com/watch?v=fPJOigUC7yU&list=PLlnvVTSJ0XwcsRgaXHeQZBhOX5KTOKaXZ)
- Creator: [Yousaf-Wajih](https://github.com/Yousaf-Wajih)

## Overview

CUB3DPP is a learning project focused on:
- 3D ray-casting techniques
- Game development fundamentals using SFML 3.0
- Modern C++ programming practices
- Computer graphics concepts

## Project Structure
```
.
├── src/         # Source files (.cpp)
├── include/     # Header files (.h, .hpp)
├── obj/         # Object files (.o)
├── bin/         # Executable files
└── CMakeLists.txt  # CMake build configuration
```

## Building the Project

### Requirements
- C++ compiler with C++17 support
- SFML 3.0
- CMake 3.10 or higher

### Build Instructions

```bash
mkdir build && cd build
cmake ..
cmake --build .
```
