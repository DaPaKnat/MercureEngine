#ifndef __VULKAN_DEBUG_HPP__
#define __VULKAN_DEBUG_HPP__

#include "CoreMinimal.hpp"

namespace Debug
{
    // ============================== [Global Methods] ============================== //

    /**
     * Setups instance debugging.
     *
     * @param p_instance        Opaque handle to the instance object.
     * @param p_messageSeverity Bitmask specifying which severity of event(s) will cause this callback to be called.
     * @param p_messageType     Bitmask specifying which type of event(s) will cause this callback to be called.
     * @param p_userData        User data to be passed to the callback.
     *
     * @return                  Whether or not a new debug messenger object could be created.
     */
    ENGINE_API void             SetupDebugging  (VkInstance const                       p_instance,
                                                 VkDebugUtilsMessageSeverityFlagsEXT    p_messageSeverity,
                                                 VkDebugUtilsMessageTypeFlagsEXT        p_messageType,
                                                 void*                                  p_userData = nullptr)   noexcept;

    /**
     * Destroys the debug messenger object.
     *
     * @param p_instance    Opaque handle to the instance object.
     */
    ENGINE_API void             FreeDebugging   (VkInstance const                       p_instance)             noexcept;

    /**
     * Checks a vulkan command return code.
     *
     * @param p_result      Vulkan command return code to check.
     * @param p_file        File where the check occured.
     * @param p_line        Line where the check occured.
     * @param p_function    Function where the check occured.
     */
    ENGINE_API VkResult         CheckResult     (VkResult                               p_result,
                                                 ANSICHAR const*                        p_file,
                                                 int32                                  p_line,
                                                 ANSICHAR const*                        p_function)             noexcept;

    /**
     * Converts an allocation scope to a readable string.
     *
     * @param p_allocationScope Allocation scope.
     */
    ENGINE_API ANSICHAR const*  ToString        (VkSystemAllocationScope                p_allocationScope)      noexcept;

    /**
     * Converts an allocation type to a readable string.
     *
     * @param p_allocationType  Allocation type.
     */
    ENGINE_API ANSICHAR const*  ToString        (VkInternalAllocationType               p_allocationType)       noexcept;

    /**
     * Converts an enumeration to track object handle types to a readable string.
     *
     * @param p_object  Enumeration to track object handle types
     */
    ENGINE_API ANSICHAR const*  ToString        (VkObjectType                           p_object)               noexcept;

    // ===================================================================================== //

    /**
     * @param p_device          Opaque handle to the logical device object.
     * @param p_objectHandle    Object to be named.
     * @param p_objectType      Type of the object to be named.
     * @param p_name            Name to apply to object.
     *
     * @return                  Whether or not a user-friendly name could be given to an object.
     */
    ENGINE_API void SetObjectName               (VkDevice const         p_device,
                                                 uint64                 p_object,
                                                 VkObjectType           p_objectType,
                                                 ANSICHAR const*        p_name)         noexcept;

    /**
     * @param p_device          Opaque handle to the logical device object.
     * @param p_objectHandle    Object to be tagged.
     * @param p_objectType      Type of the object to be named.
     * @param p_name            Numerical identifier of the tag.
     * @param p_tagSize         Number of bytes of data to attach to the object.
     * @param p_tag             Data to be associated with the object.
     *
     * @return                  Whether or not arbitrary data could be attached to an object.
     */
    ENGINE_API void SetObjectTag                (VkDevice const         p_device,
                                                 uint64                 p_object,
                                                 VkObjectType           p_objectType,
                                                 uint64                 p_name,
                                                 size_t                 p_tagSize,
                                                 void const*            p_tag)          noexcept;

    /**
     * Opens a queue debug label region.
     *
     * @param p_queue       Opaque handle to a queue object.
     * @param p_labelName   Name of the label.
     * @param p_color       RGBA color value associated with the label (ranged from 0.0 to 1.0).
     */
    ENGINE_API void BeginQueueLabelRegion       (VkQueue  const         p_queue,
                                                 ANSICHAR const*        p_labelName,
                                                 Color    const&        p_color)        noexcept;

    /**
     * Closes a queue debug label region.
     *
     * @param p_queue   Opaque handle to a queue object.
     */
    ENGINE_API void EndQueueLabelRegion         (VkQueue const          p_queue)        noexcept;

    /**
     * Inserts a label into a queue.
     *
     * @param p_queue       Opaque handle to a queue object.
     * @param p_labelName   Name of the label.
     * @param p_color       RGBA color value associated with the label (ranged from 0.0 to 1.0).
     */
    ENGINE_API void InsertQueueLabel            (VkQueue  const         p_queue,
                                                 ANSICHAR const*        p_labelName,
                                                 Color    const&        p_color)        noexcept;

    /**
     * Opens a command buffer debug label region.
     *
     * @param p_cmdBuffer   Opaque handle to a command buffer object.
     * @param p_labelName   Name of the label.
     * @param p_color       RGBA color value associated with the label (ranged from 0.0 to 1.0).
     */
    ENGINE_API void BeginCmdBufferLabelRegion   (VkCommandBuffer const  p_cmdBuffer,
                                                 ANSICHAR        const* p_labelName,
                                                 Color           const& p_color)        noexcept;

    /**
     * Closes a command buffer label region.
     *
     * @param p_cmdBuffer   Opaque handle to a command buffer object.
     */
    ENGINE_API void EndCmdBufferLabelRegion     (VkCommandBuffer const  p_cmdBuffer)    noexcept;

    /**
     * Inserts a label into a command buffer.
     *
     * @param p_cmdBuffer   Opaque handle to a command buffer object.
     * @param p_labelName   Name of the label.
     * @param p_color       RGBA color value associated with the label (ranged from 0.0 to 1.0).
     */
    ENGINE_API void InsertCmdBufferLabel        (VkCommandBuffer const  p_cmdBuffer,
                                                 ANSICHAR        const* p_labelName,
                                                 Color           const& p_color)        noexcept;

    // ===================================================================================== //

    /**
     * Gives a user-friendly name to a queue object.
     */
    INLINE void SetQueueName                (VkDevice const                 p_device,
                                             VkQueue  const                 p_queue,
                                             ANSICHAR const*                p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_queue, VK_OBJECT_TYPE_QUEUE, p_name);
    }

    /**
     * Gives a user-friendly name to a semaphore object.
     */
    INLINE void SetSemaphoreName            (VkDevice     const             p_device,
                                             VkSemaphore  const             p_semaphore,
                                             ANSICHAR     const*            p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_semaphore, VK_OBJECT_TYPE_SEMAPHORE, p_name);
    }

    /**
     * Gives a user-friendly name to a command buffer object.
     */
    INLINE void SetCommandBufferName        (VkDevice        const          p_device,
                                             VkCommandBuffer const          p_cmdBuffer,
                                             ANSICHAR        const*         p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_cmdBuffer, VK_OBJECT_TYPE_COMMAND_BUFFER, p_name);
    }

    /**
     * Gives a user-friendly name to a fence object.
     */
    INLINE void SetFenceName                (VkDevice const                 p_device,
                                             VkFence  const                 p_fence,
                                             ANSICHAR const*                p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_fence, VK_OBJECT_TYPE_FENCE, p_name);
    }

    /**
     * Gives a user-friendly name to a device memory object.
     */
    INLINE void SetDeviceMemoryName         (VkDevice        const          p_device,
                                             VkDeviceMemory  const          p_memory,
                                             ANSICHAR        const*         p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_memory, VK_OBJECT_TYPE_DEVICE_MEMORY, p_name);
    }

    /**
     * Gives a user-friendly name to a buffer object.
     */
    INLINE void SetBufferName               (VkDevice const                 p_device,
                                             VkBuffer const                 p_buffer,
                                             ANSICHAR const*                p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_buffer, VK_OBJECT_TYPE_BUFFER, p_name);
    }

    /**
     * Gives a user-friendly name to an image object.
     */
    INLINE void SetImageName                (VkDevice const                 p_device,
                                             VkImage  const                 p_image,
                                             ANSICHAR const*                p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_image, VK_OBJECT_TYPE_IMAGE, p_name);
    }

    /**
     * Gives a user-friendly name to an event object.
     */
    INLINE void SetEventName                (VkDevice const                 p_device,
                                             VkEvent  const                 p_event,
                                             ANSICHAR const*                p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_event, VK_OBJECT_TYPE_EVENT, p_name);
    }

    /**
     * Gives a user-friendly name to a query pool object.
     */
    INLINE void SetQueryPoolName            (VkDevice    const              p_device,
                                             VkQueryPool const              p_pool,
                                             ANSICHAR    const*             p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_pool, VK_OBJECT_TYPE_QUERY_POOL, p_name);
    }

    /**
     * Gives a user-friendly name to a buffer view object.
     */
    INLINE void SetBufferViewName           (VkDevice     const             p_device,
                                             VkBufferView const             p_view,
                                             ANSICHAR     const*            p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_view, VK_OBJECT_TYPE_BUFFER_VIEW, p_name);
    }

    /**
     * Gives a user-friendly name to an image view object.
     */
    INLINE void SetImageViewName            (VkDevice    const              p_device,
                                             VkImageView const              p_view,
                                             ANSICHAR    const*             p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_view, VK_OBJECT_TYPE_IMAGE_VIEW, p_name);
    }

    /**
     * Gives a user-friendly name to a shader module object.
     */
    INLINE void SetShaderModuleName         (VkDevice       const           p_device,
                                             VkShaderModule const           p_module,
                                             ANSICHAR       const*          p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_module, VK_OBJECT_TYPE_SHADER_MODULE, p_name);
    }

    /**
     * Gives a user-friendly name to a pipeline cache object.
     */
    INLINE void SetPipelineCacheName        (VkDevice        const          p_device,
                                             VkPipelineCache const          p_cache,
                                             ANSICHAR        const*         p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_cache, VK_OBJECT_TYPE_PIPELINE_CACHE, p_name);
    }

    /**
     * Gives a user-friendly name to a pipeline layout object.
     */
    INLINE void SetPipelineLayoutName       (VkDevice         const         p_device,
                                             VkPipelineLayout const         p_layout,
                                             ANSICHAR         const*        p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_layout, VK_OBJECT_TYPE_PIPELINE_LAYOUT, p_name);
    }

    /**
     * Gives a user-friendly name to a render pass object.
     */
    INLINE void SetRenderPassName           (VkDevice     const             p_device,
                                             VkRenderPass const             p_renderPass,
                                             ANSICHAR     const*            p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_renderPass, VK_OBJECT_TYPE_RENDER_PASS, p_name);
    }

    /**
     * Gives a user-friendly name to a pipeline object.
     */
    INLINE void SetPipelineName             (VkDevice   const               p_device,
                                             VkPipeline const               p_pipeline,
                                             ANSICHAR   const*              p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_pipeline, VK_OBJECT_TYPE_PIPELINE, p_name);
    }

    /**
     * Gives a user-friendly name to a pipeline layout object.
     */
    INLINE void SetDescriptorSetLayoutName  (VkDevice              const    p_device,
                                             VkDescriptorSetLayout const    p_setLayout,
                                             ANSICHAR              const*   p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_setLayout, VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, p_name);
    }

    /**
     * Gives a user-friendly name to a pipeline layout object.
     */
    INLINE void SetSamplerName              (VkDevice  const                p_device,
                                             VkSampler const                p_sampler,
                                             ANSICHAR  const*               p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_sampler, VK_OBJECT_TYPE_SAMPLER, p_name);
    }

    /**
     * Gives a user-friendly name to a descriptor pool object.
     */
    INLINE void SetDescriptorPoolName       (VkDevice         const         p_device,
                                             VkDescriptorPool const         p_pool,
                                             ANSICHAR         const*        p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_pool, VK_OBJECT_TYPE_DESCRIPTOR_POOL, p_name);
    }

    /**
     * Gives a user-friendly name to a descriptor set object.
     */
    INLINE void SetDescriptorSetName        (VkDevice        const          p_device,
                                             VkDescriptorSet const          p_set,
                                             ANSICHAR        const*         p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_set, VK_OBJECT_TYPE_DESCRIPTOR_SET, p_name);
    }

    /**
     * Gives a user-friendly name to a framebuffer object.
     */
    INLINE void SetFramebufferName          (VkDevice      const            p_device,
                                             VkFramebuffer const            p_framebuffer,
                                             ANSICHAR      const*           p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_framebuffer, VK_OBJECT_TYPE_FRAMEBUFFER, p_name);
    }

    /**
     * Gives a user-friendly name to a command pool object.
     */
    INLINE void SetCommandPoolName          (VkDevice      const            p_device,
                                             VkCommandPool const            p_pool,
                                             ANSICHAR      const*           p_name)     noexcept
    {
        SetObjectName(p_device, (uint64)p_pool, VK_OBJECT_TYPE_COMMAND_POOL, p_name);
    }

}   // !namespace Debug

#define VK_CHECK_RESULT(result) Debug::CheckResult(result, __FILE__, __LINE__, __PRETTY_FUNCTION__)

#endif // !__VULKAN_DEBUG_HPP__