#pragma once
#include <vulkan/vulkan.h>

#define MAX_FRAMES_IN_FLIGHT 2

struct QueueFamilyIndices{
    uint32_t graphics = UINT32_MAX;
    uint32_t present = UINT32_MAX;
};

struct SynchronisationObjects{
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;
};

struct VulkanObjects{
    VkInstance instance;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties physicalDeviceProperties;
    VkPhysicalDeviceFeatures physicalDeviceFeatures;
    QueueFamilyIndices queueFamilyIndices;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR surfaceFormat;
    VkPresentModeKHR presentMode;
    VkExtent2D swapchainExtent;
    VkSwapchainKHR swapchain;
    VkImage* swapchainImages;
    uint32_t swapchainImageCount;
    VkImageView* swapchainImageViews;
    VkRenderPass renderPass; 
    VkPipelineLayout graphicsPipelineLayout;
    VkPipeline graphicsPipeline;
    VkFramebuffer* swapchainFramebuffers;
    VkCommandPool commandPool;
    VkCommandBuffer commandBuffers[MAX_FRAMES_IN_FLIGHT];
    SynchronisationObjects syncObjects[MAX_FRAMES_IN_FLIGHT];

    void cleanUp();
};