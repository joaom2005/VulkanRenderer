# VulkanRenderer
A minimal Vulkan renderer project that demonstrates setting up a Vulkan instance, device, swapchain, 
and a basic render loop. Intended as a starting point for learning or extending Vulkan rendering with a 
C++20 codebase and CMake + Ninja build.

Prerequisites
- Vulkan SDK installed and VULKAN_SDK environment variable set.
- On Windows, you need vcpkg to install the dependencies (glfw, glm and tinyobjloader). 
On linux you can install these with your package manager.
- CMake >= 3.29 (project tested with CMake 4.2.3-msvc3).
- Ninja build system.
- A C\++20 toolchain (compiler supporting C++20). 

To build on Windows:
1. Clone the repo
2. Open Visual Studio and build/run
3. Run `cmake -S . -B build` and `then cmake --build build`

Notes
- Ensure your GPU drivers support Vulkan and are up to date.
- If CMake cannot find Vulkan, point CMake to the SDK:
- Linux/macOS: export VULKAN_SDK=/path/to/vulkan
- Windows (PowerShell): $env:VULKAN_SDK = 'C:\path\to\vulkan'
