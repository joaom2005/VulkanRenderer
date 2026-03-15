cmake_minimum_required (VERSION 3.29) # CMake 3.29 is required for C++20 module support.

project (VulkanRenderer VERSION 0.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Add option to enable/disable C++ 20 module
option(ENABLE_CPP20_MODULE "Enable C++ 20 module support for Vulkan" OFF)

# Enable C++ module dependency scanning only if C++ 20 module is enabled
if(ENABLE_CPP20_MODULE)
  set(CMAKE_CXX_SCAN_FOR_MODULES ON)
endif()

find_package (glfw3 CONFIG REQUIRED)
find_package (glm REQUIRED)
find_package (Vulkan 1.4.335 REQUIRED)      # Require Vulkan SDK version 1.4.335 or higher

add_executable(${PROJECT_NAME} "src/main.cpp" "src/HelloTriangleApplication.cpp")

# Target link libraries
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} glm::glm)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)

# Make all include directories available
target_include_directories(${PROJECT_NAME} PRIVATE glfw/include)
target_include_directories(${PROJECT_NAME} PRIVATE vendor/glm/glm)