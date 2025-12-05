#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <optional>

#define VULKAN_API_VERSION VK_API_VERSION_1_3

// --- Helper: Check instance extension support ---
bool checkExtensionSupport(const std::vector<const char*>& requiredExtensions) {
    uint32_t extensionCount;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

    std::cout << "--- Available Vulkan Extensions (" << extensionCount << ") ---" << std::endl;
    for (auto& ext : availableExtensions) {
        std::cout << "  " << ext.extensionName << std::endl;
    }
    std::cout << "---------------------------------------------------" << std::endl;

    for (const char* required : requiredExtensions) {
        bool found = false;
        for (auto& ext : availableExtensions) {
            if (strcmp(required, ext.extensionName) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            std::cerr << "ERROR: Required Vulkan extension not found: " << required << std::endl;
            return false;
        }
    }
    return true;
}

// --- Create Vulkan Instance ---
VkInstance createInstance() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    std::cout << "--- GLFW Required Extensions (" << glfwExtensionCount << ") ---" << std::endl;
    for (uint32_t i = 0; i < glfwExtensionCount; i++) {
        std::cout << "  " << glfwExtensions[i] << std::endl;
    }
    std::cout << "---------------------------------------------------" << std::endl;

    if (!checkExtensionSupport(extensions)) {
        throw std::runtime_error("Not all required Vulkan extensions are supported.");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Penguin Physical Engine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VULKAN_API_VERSION;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    VkInstance instance;
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance!");
    }

    std::cout << "Vulkan instance successfully created!" << std::endl;
    return instance;
}

// --- Find queue family with graphics support ---
std::optional<uint32_t> findGraphicsQueueFamily(VkPhysicalDevice device) {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> families(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, families.data());

    for (uint32_t i = 0; i < queueFamilyCount; i++) {
        if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            return i;
        }
    }
    return std::nullopt;
}

// --- Create Logical Device + Graphics Queue ---
VkDevice createLogicalDevice(VkPhysicalDevice physicalDevice, uint32_t& outGraphicsFamily, VkQueue& outGraphicsQueue) {
    auto graphicsFamily = findGraphicsQueueFamily(physicalDevice);
    if (!graphicsFamily.has_value()) {
        throw std::runtime_error("No graphics queue family found!");
    }

    outGraphicsFamily = graphicsFamily.value();

    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = graphicsFamily.value();
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;
    createInfo.enabledLayerCount = 0;

    VkDevice device;
    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device!");
    }

    vkGetDeviceQueue(device, graphicsFamily.value(), 0, &outGraphicsQueue);

    std::cout << "Logical device successfully created!" << std::endl;
    return device;
}

// --- Main ---
int main() {
    GLFWwindow* window = nullptr;
    VkInstance instance = VK_NULL_HANDLE;

    try {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return -1;
        }

        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window = glfwCreateWindow(800, 600, "Penguin Physical Engine", nullptr, nullptr);
        if (!window) throw std::runtime_error("Failed to create GLFW window");

        instance = createInstance();

        VkSurfaceKHR surface;
        glfwCreateWindowSurface(instance, window ,nullptr, &surface);

        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        if (deviceCount == 0) throw std::runtime_error("No Vulkan-compatible GPUs found!");

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
        VkPhysicalDevice physicalDevice = devices[0];

        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(physicalDevice, &props);
        std::cout << "Using GPU: " << props.deviceName << std::endl;

        uint32_t graphicsFamily;
        VkQueue graphicsQueue;
        VkDevice device = createLogicalDevice(physicalDevice, graphicsFamily, graphicsQueue);

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }

        vkDestroyDevice(device, nullptr);
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();

    } catch (const std::exception& e) {
        std::cerr << "Runtime Error: " << e.what() << std::endl;
        if (instance != VK_NULL_HANDLE) vkDestroyInstance(instance, nullptr);
        if (window) glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    return 0;
}
