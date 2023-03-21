#include "draw.h"
#include <cstdio>
#include <cstdlib>

void recordCommandBuffer(
    VkCommandBuffer commandBuffer, 
    VkRenderPass renderPass, 
    VkFramebuffer* swapChainFramebuffers, 
    uint32_t swapChainFramebuffersIndex,
    VkExtent2D swapChainExtent,
    VkPipeline graphicsPipeline
){
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if(vkBeginCommandBuffer(commandBuffer, &beginInfo)!= VK_SUCCESS){
        printf("Failed to begin Command Buffer Recording!\n");
        exit(EXIT_FAILURE);
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = swapChainFramebuffers[swapChainFramebuffersIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChainExtent;
    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapChainExtent.width;
    viewport.height = (float)swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    vkCmdDraw(commandBuffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS){
        printf("Failed to end Command Buffer!\n");
        exit(EXIT_FAILURE);
    }
}

void drawFrame(
    VkDevice device, 
    SynchronisationObjects syncObjects, 
    VkSwapchainKHR swapchain, 
    VkExtent2D swapchainExtent,
    VkCommandBuffer commandBuffer, 
    VkRenderPass renderPass,
    VkFramebuffer *swapchainFramebuffers,
    VkPipeline graphicsPipeline,
    VkQueue graphicsQueue
){
    vkWaitForFences(device, 1, &syncObjects.inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(device, 1, &syncObjects.inFlightFence);

    uint32_t swapchainImageIndex;
    vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, syncObjects.imageAvailableSemaphore, VK_NULL_HANDLE, &swapchainImageIndex);
    
    vkResetCommandBuffer(commandBuffer, 0);
    recordCommandBuffer(commandBuffer, renderPass, swapchainFramebuffers, swapchainImageIndex, swapchainExtent, graphicsPipeline);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &syncObjects.imageAvailableSemaphore;
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.pWaitDstStageMask = waitStages;
    //Each entry in the waitStages array corresponds to the semaphore with the same index in pWaitSemaphores.
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &syncObjects.renderFinishedSemaphore;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, syncObjects.inFlightFence) != VK_SUCCESS) {
        printf("Failed to submit to Graphics Queue!\n");
        exit(EXIT_FAILURE);
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &syncObjects.renderFinishedSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain;
    presentInfo.pImageIndices = &swapchainImageIndex;

    vkQueuePresentKHR(graphicsQueue, &presentInfo);
}