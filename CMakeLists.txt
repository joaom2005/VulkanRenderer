cmake_minimum_required(VERSION 3.29)  # 3.29+ required for C++20 module support
project(VulkanRenderer VERSION 0.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# ── Options ───────────────────────────────────────────────────────────────────

# Disabled by default; enabling requires a Vulkan SDK build that ships vulkan.cppm
option(ENABLE_CPP20_MODULE "Enable C++20 module support for Vulkan" ON)

# Module dependency scanning is expensive — only pay the cost when modules are used
if(ENABLE_CPP20_MODULE)
  set(CMAKE_CXX_SCAN_FOR_MODULES ON)
endif()

# ── Dependencies ──────────────────────────────────────────────────────────────

find_package(glfw3       CONFIG REQUIRED)
find_package(glm                REQUIRED)
find_package(Vulkan      1.4.335 REQUIRED)  # Minimum SDK version that ships vulkan.cppm
find_package(tinyobjloader      REQUIRED)
find_package(KTX                REQUIRED)
find_package(stb                REQUIRED)

# Header-only; no CMake package, so locate the directory manually
find_path(TINYGLTF_INCLUDE_DIRS "tiny_gltf.h")

set(STB_INCLUDEDIR ${stb_INCLUDE_DIRS})

# ── Vulkan C++20 Module ───────────────────────────────────────────────────────

if(ENABLE_CPP20_MODULE)
  add_library(VulkanCppModule)
  add_library(Vulkan::cppm ALIAS VulkanCppModule)

  target_compile_definitions(VulkanCppModule PUBLIC
    VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1   # Load function pointers at runtime via DispatchLoaderDynamic
    VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1    # Disable aggregate constructors; forces designated-initialiser syntax
  )

  target_include_directories(VulkanCppModule PUBLIC "${Vulkan_INCLUDE_DIR}")
  target_link_libraries(VulkanCppModule PUBLIC Vulkan::Vulkan)
  set_target_properties(VulkanCppModule PROPERTIES CXX_STANDARD 20)

  if(MSVC)
    target_compile_options(VulkanCppModule PRIVATE
      /std:c++latest    # Modules need the latest language version on MSVC
      /permissive-      # Strict standards conformance; required for modules to parse correctly
      /Zc:__cplusplus   # Emit the real __cplusplus value instead of 199711L
      /EHsc             # Standard C++ exception handling
      /Zc:preprocessor  # Conforming preprocessor; the legacy one mishandles some module headers
      /translateInclude # Treat #include of known module headers as import
    )
  endif()

  # Compile vulkan.cppm as a named C++ module (FILE_SET CXX_MODULES)
  target_sources(VulkanCppModule PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES
    BASE_DIRS "${Vulkan_INCLUDE_DIR}"
    FILES     "${Vulkan_INCLUDE_DIR}/vulkan/vulkan.cppm"
  )

  # Also add as a regular source so IDEs and non-module build paths see the file
  target_sources(VulkanCppModule PRIVATE
    "${Vulkan_INCLUDE_DIR}/vulkan/vulkan.cppm"
  )
else()
  # Stub target that satisfies Vulkan::cppm references without pulling in module machinery
  add_library(VulkanCppModule INTERFACE)
  add_library(Vulkan::cppm ALIAS VulkanCppModule)
  target_link_libraries(VulkanCppModule INTERFACE Vulkan::Vulkan)
  target_compile_definitions(VulkanCppModule INTERFACE
    VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
    VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
  )
endif()

# ── Shader Compilers ──────────────────────────────────────────────────────────

# Wrap glslangValidator as an imported target so it can be used in add_custom_command
add_executable(glslang::validator IMPORTED)
find_program(GLSLANG_VALIDATOR "glslangValidator" HINTS $ENV{VULKAN_SDK}/bin REQUIRED)
set_property(TARGET glslang::validator PROPERTY IMPORTED_LOCATION "${GLSLANG_VALIDATOR}")

find_program(SLANGC_EXECUTABLE slangc HINTS $ENV{VULKAN_SDK}/bin REQUIRED)

# ── Main Executable ───────────────────────────────────────────────────────────

add_executable(${PROJECT_NAME}
  "src/main.cpp"
  "src/HelloTriangleApplication.cpp"
)

target_link_libraries(${PROJECT_NAME}
  glfw ${GLFW_LIBRARIES}
  glm::glm
  Vulkan::Vulkan
)

target_include_directories(${PROJECT_NAME} PRIVATE
  glfw/include
  vendor/glm/glm
  ${TINYGLTF_INCLUDE_DIRS}
)