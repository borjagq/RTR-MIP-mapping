# Real-time MIP map comparison

A visualisation and comparison in OpenGL of the effects that different MIP-mapping models have.

## About this repo

MIP maps are precomputed, hierarchical texture representations used in computer graphics, particularly in OpenGL, to optimize rendering performance. This OpenGL project focuses on providing a practical visualization and comparison of MIP map configurations, showcasing different levels of detail for textures at varying distances. The project allows users to interactively explore the impact of MIP map settings on visual quality and performance.

The scene consists of an object rotating around its centre. The displayed object shows a texture using the specified mipmap parameters in the GUI. There are three possible models: a plane, a sphere and a cube. All of them have been created using Blender. The same program also generated normals and UV coordinates. Finally, all the textures were created using illustrator to obtain the highest-frequency images I could get with the sharpest edges.

## Demo video

[![Watch the video](https://img.youtube.com/vi/lL1ZsqRfYSY/0.jpg)](https://www.youtube.com/watch?v=lL1ZsqRfYSY)

## Getting Started

### Dependencies

- [Xcode 14.3.1](https://developer.apple.com/xcode/)
- OpenGL 4.1 (included in macOS)
- [Glew 2.2.0](https://glew.sourceforge.net)
- [GLFW 3.3.8](https://github.com/glfw/glfw)
- [Assimp 5.3.1](https://github.com/assimp/assimp)
- [GLM 1.0.0](https://github.com/g-truc/glm)

### Installing

1. Clone this repository.

```sh
git clone https://github.com/borjagq/RTR-MIP-mapping/
```

2. Open the project ```Lab4.xcodeproj```

3. Build and run

# License

This project is licensed under the MIT License - see the [LICENSE](https://github.com/borjagq/RTR-MIP-mapping/LICENSE/) file for details

# Acknowledgments

This repository incorporates essential components from the open-source libraries [stb](https://github.com/nothings/stb) and [Dear ImGui](https://github.com/ocornut/imgui). These libraries provide the file read/write and GUI functionalities.  For further details about these libraries, visit their GitHub repositories.
