#ifndef __VULKAN_QUEUE_HPP__
#define __VULKAN_QUEUE_HPP__

#include "Vulkan/Vulkan.hpp"

class ENGINE_API Queue : public UniqueObject
{
    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        Queue           () = delete;

        explicit Queue  (uint32  p_queueFamily,
                         VkQueue p_handle);

        ~Queue          () = default;

    // ============================== [Public Local Methods] ============================== //

        /**
         * @thread_safety This function may be called from any thread.
         */
        void    Submit      (VkCommandBuffer const              p_cmdBuffer,
                             VkFence         const              p_fence = VK_NULL_HANDLE)   noexcept;

        /**
         * @thread_safety This function may be called from any thread.
         */
        void    Submit      (VkPipelineStageFlags               p_waitStage,
                             VkSemaphore     const              p_waitSemaphore,
                             VkSemaphore     const              p_signalSemaphore,
                             VkCommandBuffer const              p_cmdBuffer,
                             VkFence         const              p_fence = VK_NULL_HANDLE)   noexcept;

        /**
         * @thread_safety This function may be called from any thread.
         */
        void    Present     (uint32                             p_imageIndex,
                             VkSwapchainKHR const               p_swapchain,
                             VkSemaphore    const               p_semaphore)                noexcept;

        /**
         * @thread_safety This function may be called from any thread.
         */
        void    Present     (uint32                             p_imageIndex,
                             VkSwapchainKHR           const     p_swapchain,
                             std::vector<VkSemaphore> const&    p_semaphores)               noexcept;

    private:

    // ============================== [Private Local Properties] ============================== //

        std::mutex  m_mutex;

        uint32      m_queueFamily;
        
        VkQueue     m_handle;

};  // !class Queue

#endif // !__VULKAN_QUEUE_HPP__