[stb](https://github.com/nothings/stb) <br>
[glm](https://glm.g-truc.net/0.9.9/index.html) <br>
[glfw3](https://github.com/glfw/glfw) <br>
[Vulkan](https://www.khronos.org/vulkan/) <br>
[jsoncpp](https://github.com/open-source-parsers/jsoncpp) <br>
[tinyobjloader](https://github.com/tinyobjloader/tinyobjloader) <br>
[openal-soft](https://github.com/kcat/openal-soft) <br>
[gli](https://github.com/g-truc/gli) <br>
[freealut](https://github.com/vancegroup/freealut) submodule <br>

___
# How to?
1. install vcpkg
2. config cmake project

### Windows
```cmd
vcpkg install stb stb:x64-windows glm glm:x64-windows glfw3 glfw3:x64-windows vulkan vulkan:x64-windows jsoncpp jsoncpp:x64-windows tinyobjloader tinyobjloader:x64-windows openal-soft openal-soft:x64-windows gli gli:x64-windows
```
### macOS, Linux
```shell script
vcpkg install stb glm glfw3 vulkan jsoncpp tinyobjloader openal-soft gli
```