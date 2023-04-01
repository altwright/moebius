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
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout graphicsPipelineLayout;
    VkPipeline graphicsPipeline;
    VkFramebuffer* swapchainFramebuffers;
    VkCommandPool commandPool;
    VkCommandPool transientCommandPool;
    VkCommandBuffer commandBuffers[MAX_FRAMES_IN_FLIGHT];
    SynchronisationObjects syncObjects[MAX_FRAMES_IN_FLIGHT];
    VkPhysicalDeviceMemoryProperties memProperties;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    VkBuffer uniformBuffers[MAX_FRAMES_IN_FLIGHT];
    VkDeviceMemory uniformBuffersMemory[MAX_FRAMES_IN_FLIGHT];
    void* uniformBuffersMapped[MAX_FRAMES_IN_FLIGHT];

    void cleanUp();
};

uint32_t findMemoryType(
    VkPhysicalDeviceMemoryProperties *memProperties, 
    uint32_t typeFilter, 
    VkMemoryPropertyFlags propertyFlags
);

void createBuffer(
    VkDevice device, 
    VkPhysicalDeviceMemoryProperties* memProperties,
    VkBufferUsageFlags usage, 
    VkMemoryPropertyFlags properties,
    VkDeviceSize bufferSize, 
    VkBuffer *buffer, 
    VkDeviceMemory *memory
);

void copyBuffer(
    VkDevice device,
    VkCommandPool commandPool,
    VkQueue transferQueue,
    VkBuffer srcBuffer, 
    VkBuffer dstBuffer, 
    VkDeviceSize size
);