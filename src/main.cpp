#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <iostream>
#include <stdexcept>
#include <vector>

#define VK_API_VERSION_1_4 VK_MAKE_API_VERSION(0, 1, 4, 0)

void createInstance() {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Penguin Physical Engine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_4; 
    
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    //createInfo.enabledExtensionCount = static_count<uint32_t>(extensions.size()); 
    //createInfo.ppEnabledExtensionsNames = extensions.data();

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);



    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0; 
    
    std::cout << "--- Required GLFW Extensions (" << glfwExtensionCount << ") ---" << std::endl;
    for (uint32_t i = 0; i < glfwExtensionCount; i++) {
        std::cout << glfwExtensions[i] << std::endl;
    }
    std::cout << "---------------------------------------------------" << std::endl;

    VkInstance instance = VK_NULL_HANDLE;
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance!");
    }
    
    std::cout << "Vulkan instance successfully created!" << std::endl;
}

int main() {
    try {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return -1;
        }

        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        
        GLFWwindow* window = glfwCreateWindow(800, 600, "Penguin Physical Engine", nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            std::cerr << "Failed to create GLFW window" << std::endl;
            return -1;
        }
        
        createInstance();

        while(!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }

        glfwDestroyWindow(window);
        glfwTerminate();
        
    } catch (const std::exception& e) {
        std::cerr << "Runtime Error: " << e.what() << std::endl;
        glfwTerminate();
        return -1;
    }
    return 0;
}
