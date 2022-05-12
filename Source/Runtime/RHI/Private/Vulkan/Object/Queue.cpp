#include "PCH.hpp"

#include "Vulkan/Object/Queue.hpp"
#include "Vulkan/Object/Swapchain.hpp"

// ============================== [Public Constructor] ============================== //

Queue::Queue    (uint32  p_queueFamily,
                 VkQueue p_handle) :
    m_queueFamily { p_queueFamily },
    m_handle      { p_handle }
{

}

// ============================== [Public Local Methods] ============================== //

void    Queue::Submit   (VkCommandBuffer const p_cmdBuffer,
                         VkFence         const p_fence) noexcept
{
    VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };

    submitInfo.commandBufferCount = 1u;
    submitInfo.pCommandBuffers    = &p_cmdBuffer;

    {
        std::lock_guard lock(m_mutex);

        VK_CHECK_RESULT(vkQueueSubmit(m_handle, 1u, &submitInfo, p_fence));
    }
}

void    Queue::Submit   (VkPipelineStageFlags       p_waitStage,
                         VkSemaphore          const p_waitSemaphore,
                         VkSemaphore          const p_signalSemaphore,
                         VkCommandBuffer      const p_cmdBuffer,
                         VkFence              const p_fence) noexcept
{
    VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };

    submitInfo.waitSemaphoreCount   = 1u;
    submitInfo.pWaitSemaphores      = &p_waitSemaphore;
    submitInfo.pWaitDstStageMask    = &p_waitStage;
    submitInfo.commandBufferCount   = 1u;
    submitInfo.pCommandBuffers      = &p_cmdBuffer;
    submitInfo.signalSemaphoreCount = 1u;
    submitInfo.pSignalSemaphores    = &p_signalSemaphore;

    {
        std::lock_guard lock(m_mutex);

        VK_CHECK_RESULT(vkQueueSubmit(m_handle, 1u, &submitInfo, p_fence));
    }
}

void    Queue::Present  (uint32               p_imageIndex,
                         VkSwapchainKHR const p_swapchain,
                         VkSemaphore    const p_semaphore) noexcept
{
    VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };

    presentInfo.waitSemaphoreCount = 1u;
    presentInfo.pWaitSemaphores    = &p_semaphore;
    presentInfo.swapchainCount     = 1u;
    presentInfo.pSwapchains        = &p_swapchain;
    presentInfo.pImageIndices      = &p_imageIndex;

    {
        std::lock_guard lock(m_mutex);

        VkResult result = vkQueuePresentKHR(m_handle, &presentInfo);

        if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR)
            return;

        VK_CHECK_RESULT(result);
    }
}

void    Queue::Present  (uint32                          p_imageIndex,
                         VkSwapchainKHR           const  p_swapchain,
                         std::vector<VkSemaphore> const& p_semaphores) noexcept
{
    VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };

    presentInfo.waitSemaphoreCount = static_cast<uint32>(p_semaphores.size());
    presentInfo.pWaitSemaphores    = p_semaphores.data();
    presentInfo.swapchainCount     = 1u;
    presentInfo.pSwapchains        = &p_swapchain;
    presentInfo.pImageIndices      = &p_imageIndex;

    {
        std::lock_guard lock(m_mutex);

        VkResult result = vkQueuePresentKHR(m_handle, &presentInfo);

        if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR)
            return;

        VK_CHECK_RESULT(result);
    }
}