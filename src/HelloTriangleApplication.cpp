#include "HelloTriangleApplication.hpp"

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

void createInstance(vk::raii::Context& context, vk::raii::Instance& instance) {
    constexpr vk::ApplicationInfo appInfo{ .pApplicationName = "Hello Triangle",
                                        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                                        .pEngineName = "No Engine",
                                        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
                                        .apiVersion = vk::ApiVersion14 };

    // Get the required instance extensions from GLFW.
    uint32_t glfwExtensionCount = 0;
    auto     glfwExtensions     = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    // Check if the required GLFW extensions are supported by the Vulkan implementation.
    auto extensionProperties = context.enumerateInstanceExtensionProperties();
    for (uint32_t i = 0; i < glfwExtensionCount; ++i)
    {
        if (std::ranges::none_of(extensionProperties,
            [glfwExtension = glfwExtensions[i]](auto const& extensionProperty)
            { return strcmp(extensionProperty.extensionName, glfwExtension) == 0; }))
        {
            throw std::runtime_error("Required GLFW extension not supported: " + std::string(glfwExtensions[i]));
        }
    }

    vk::InstanceCreateInfo createInfo{
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = glfwExtensionCount,
        .ppEnabledExtensionNames = glfwExtensions };

    instance = vk::raii::Instance(context, createInfo);

#ifndef NDEBUG
    // Get available extensions and show them
    auto extensions = context.enumerateInstanceExtensionProperties();
    std::cout << "available extensions:\n";

    for (const auto& extension : extensions) {
        std::cout << '\t' << extension.extensionName << '\n';
    }
#endif // NDEBUG
}

void HelloTriangleApplication::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void HelloTriangleApplication::initWindow() {
    glfwInit();

    // Setup some window hints
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);   // Tell GLFW to not create a OpenGL context
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);     // Disable resizing

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void HelloTriangleApplication::initVulkan() {
    createInstance(context, instance);
}

void HelloTriangleApplication::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

void HelloTriangleApplication::cleanup() {
    glfwDestroyWindow(window);

    glfwTerminate();
}