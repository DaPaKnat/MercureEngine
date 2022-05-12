#include "PCH.hpp"

#include "Vulkan/Utilities/Loader.hpp"
#include "Vulkan/Utilities/Debug.hpp"
#include "Vulkan/Utilities/Function.hpp"

// ============================== [Global Properties] ============================== //

/** Opaque handle to the debug messenger object. */
static VkDebugUtilsMessengerEXT DebugMessenger = VK_NULL_HANDLE;

// ============================== [Global Methods] ============================== //

void            Debug::SetupDebugging   (VkInstance const                    p_instance,
                                         VkDebugUtilsMessageSeverityFlagsEXT p_messageSeverity,
                                         VkDebugUtilsMessageTypeFlagsEXT     p_messageType,
                                         void*                               p_userData) noexcept
{
    // Loads the necessary function pointers.
    CreateDebugUtilsMessengerEXT  = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT> (vkGetInstanceProcAddr(p_instance, "vkCreateDebugUtilsMessengerEXT"));
    DestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(p_instance, "vkDestroyDebugUtilsMessengerEXT"));
    SetDebugUtilsObjectNameEXT    = reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>   (vkGetInstanceProcAddr(p_instance, "vkSetDebugUtilsObjectNameEXT"));
    SetDebugUtilsObjectTagEXT     = reinterpret_cast<PFN_vkSetDebugUtilsObjectTagEXT>    (vkGetInstanceProcAddr(p_instance, "vkSetDebugUtilsObjectTagEXT"));
    QueueBeginDebugUtilsLabelEXT  = reinterpret_cast<PFN_vkQueueBeginDebugUtilsLabelEXT> (vkGetInstanceProcAddr(p_instance, "vkQueueBeginDebugUtilsLabelEXT"));
    QueueEndDebugUtilsLabelEXT    = reinterpret_cast<PFN_vkQueueEndDebugUtilsLabelEXT>   (vkGetInstanceProcAddr(p_instance, "vkQueueEndDebugUtilsLabelEXT"));
    QueueInsertDebugUtilsLabelEXT = reinterpret_cast<PFN_vkQueueInsertDebugUtilsLabelEXT>(vkGetInstanceProcAddr(p_instance, "vkQueueInsertDebugUtilsLabelEXT"));
    CmdBeginDebugUtilsLabelEXT    = reinterpret_cast<PFN_vkCmdBeginDebugUtilsLabelEXT>   (vkGetInstanceProcAddr(p_instance, "vkCmdBeginDebugUtilsLabelEXT"));
    CmdEndDebugUtilsLabelEXT      = reinterpret_cast<PFN_vkCmdEndDebugUtilsLabelEXT>     (vkGetInstanceProcAddr(p_instance, "vkCmdEndDebugUtilsLabelEXT"));
    CmdInsertDebugUtilsLabelEXT   = reinterpret_cast<PFN_vkCmdInsertDebugUtilsLabelEXT>  (vkGetInstanceProcAddr(p_instance, "vkCmdInsertDebugUtilsLabelEXT"));

    // In case a debug messenger object already exists, destroys it before creating a new one.
    if (DebugMessenger)
        FreeDebugging(p_instance);

    // Creates the debug messenger object.
    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCI = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };

    debugUtilsMessengerCI.messageSeverity = p_messageSeverity;
    debugUtilsMessengerCI.messageType     = p_messageType;
    debugUtilsMessengerCI.pfnUserCallback = vkDebugCallback;
    debugUtilsMessengerCI.pUserData       = p_userData;

    VK_CHECK_RESULT(vkCreateDebugUtilsMessengerEXT(p_instance, &debugUtilsMessengerCI, nullptr, &DebugMessenger));

    return;
}

void            Debug::FreeDebugging    (VkInstance const p_instance) noexcept
{
    vkDestroyDebugUtilsMessengerEXT(p_instance, DebugMessenger, nullptr);

    DebugMessenger = VK_NULL_HANDLE;

    return;
}

VkResult        Debug::CheckResult      (VkResult        p_result,
                                         ANSICHAR const* p_file,
                                         int32           p_line,
                                         ANSICHAR const* p_function) noexcept
{
    ANSICHAR const* message = "";

    #ifndef NDEBUG

    switch (p_result)
    {
    #define RESULT(res) case VK_##res: message = #res; break
            RESULT(SUCCESS);
            RESULT(NOT_READY);
            RESULT(TIMEOUT);
            RESULT(EVENT_SET);
            RESULT(EVENT_RESET);
            RESULT(INCOMPLETE);
            RESULT(ERROR_OUT_OF_HOST_MEMORY);
            RESULT(ERROR_OUT_OF_DEVICE_MEMORY);
            RESULT(ERROR_INITIALIZATION_FAILED);
            RESULT(ERROR_DEVICE_LOST);
            RESULT(ERROR_MEMORY_MAP_FAILED);
            RESULT(ERROR_LAYER_NOT_PRESENT);
            RESULT(ERROR_EXTENSION_NOT_PRESENT);
            RESULT(ERROR_FEATURE_NOT_PRESENT);
            RESULT(ERROR_INCOMPATIBLE_DRIVER);
            RESULT(ERROR_TOO_MANY_OBJECTS);
            RESULT(ERROR_FORMAT_NOT_SUPPORTED);
            RESULT(ERROR_FRAGMENTED_POOL);
            RESULT(ERROR_OUT_OF_POOL_MEMORY);
            RESULT(ERROR_INVALID_EXTERNAL_HANDLE);
            RESULT(ERROR_SURFACE_LOST_KHR);
            RESULT(ERROR_NATIVE_WINDOW_IN_USE_KHR);
            RESULT(SUBOPTIMAL_KHR);
            RESULT(ERROR_OUT_OF_DATE_KHR);
            RESULT(ERROR_INCOMPATIBLE_DISPLAY_KHR);
            RESULT(ERROR_VALIDATION_FAILED_EXT);
            RESULT(ERROR_INVALID_SHADER_NV);
            RESULT(ERROR_FRAGMENTATION_EXT);
            RESULT(ERROR_NOT_PERMITTED_EXT);
    #undef  RESULT

        default:
            message = "UNKNOWN_ERROR_CODE";            
            break;
    }

    if (p_result == 0)
        LOG(LogRHI, Log,   "%s in function %s in line %d in file %s", message, p_function, p_line, p_file);

    else if (p_result > 0)
        LOG(LogRHI, Error, "%s in function %s in line %d in file %s", message, p_function, p_line, p_file);

    else
        LOG(LogRHI, Fatal, "%s in function %s in line %d in file %s", message, p_function, p_line, p_file);

    #else

    if (p_result < 0)
        LOG(LogRHI, Fatal, "Fatal in function %s in line %d in file %s", "", p_function, p_line, p_file);

    #endif

    return p_result;
}

ANSICHAR const* Debug::ToString         (VkSystemAllocationScope p_allocationScope) noexcept
{
    switch (p_allocationScope)
    {
    #define SCOPE(scope) case VK_SYSTEM_##scope: return #scope;
            SCOPE(ALLOCATION_SCOPE_COMMAND);
            SCOPE(ALLOCATION_SCOPE_OBJECT);
            SCOPE(ALLOCATION_SCOPE_CACHE);
            SCOPE(ALLOCATION_SCOPE_DEVICE);
            SCOPE(ALLOCATION_SCOPE_INSTANCE);
    #undef  SCOPE
    }

    return "UNKNOWN_ALLOCATION_SCOPE";
}

ANSICHAR const* Debug::ToString         (VkInternalAllocationType p_allocationType) noexcept
{
    switch (p_allocationType)
    {
    #define TYPE(type) case VK_INTERNAL_##type: return #type;
            TYPE(ALLOCATION_TYPE_EXECUTABLE);
    #undef  TYPE
    }

    return "UNKNOWN_ALLOCATION_TYPE";
}

ANSICHAR const* Debug::ToString         (VkObjectType p_object) noexcept
{
    switch (p_object)
    {
    #define OBJECT(object) case VK_OBJECT_TYPE_##object: return #object;
            OBJECT(UNKNOWN);
            OBJECT(INSTANCE);
            OBJECT(PHYSICAL_DEVICE);
            OBJECT(DEVICE);
            OBJECT(QUEUE);
            OBJECT(SEMAPHORE);
            OBJECT(COMMAND_BUFFER);
            OBJECT(FENCE);
            OBJECT(DEVICE_MEMORY);
            OBJECT(BUFFER);
            OBJECT(IMAGE);
            OBJECT(EVENT);
            OBJECT(QUERY_POOL);
            OBJECT(BUFFER_VIEW);
            OBJECT(IMAGE_VIEW);
            OBJECT(SHADER_MODULE);
            OBJECT(PIPELINE_CACHE);
            OBJECT(PIPELINE_LAYOUT);
            OBJECT(RENDER_PASS);
            OBJECT(PIPELINE);
            OBJECT(DESCRIPTOR_SET_LAYOUT);
            OBJECT(SAMPLER);
            OBJECT(DESCRIPTOR_POOL);
            OBJECT(DESCRIPTOR_SET);
            OBJECT(FRAMEBUFFER);
            OBJECT(COMMAND_POOL);
            OBJECT(SAMPLER_YCBCR_CONVERSION);
            OBJECT(DESCRIPTOR_UPDATE_TEMPLATE);
            OBJECT(SURFACE_KHR);
            OBJECT(SWAPCHAIN_KHR);
            OBJECT(DISPLAY_KHR);
            OBJECT(DISPLAY_MODE_KHR);
            OBJECT(DEBUG_REPORT_CALLBACK_EXT);
            OBJECT(OBJECT_TABLE_NVX);
            OBJECT(INDIRECT_COMMANDS_LAYOUT_NVX);
            OBJECT(DEBUG_UTILS_MESSENGER_EXT);
            OBJECT(VALIDATION_CACHE_EXT);
    #undef  OBJECT
    }

    return "UNKNOWN_OBJECT_TYPE";
}

// ============================================================================== //

void    Debug::SetObjectName                (VkDevice const  p_device,
                                             uint64          p_objectHandle,
                                             VkObjectType    p_objectType,
                                             ANSICHAR const* p_name) noexcept
{
    #ifndef NDEBUG

    VkDebugUtilsObjectNameInfoEXT nameInfo = { VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT };
    
    nameInfo.objectType   = p_objectType;
    nameInfo.objectHandle = p_objectHandle;
    nameInfo.pObjectName  = p_name;

    VK_CHECK_RESULT(vkSetDebugUtilsObjectNameEXT(p_device, &nameInfo));

    #endif
}

void    Debug::SetObjectTag                 (VkDevice const p_device,
                                             uint64         p_objectHandle,
                                             VkObjectType   p_objectType,
                                             uint64         p_name,
                                             size_t         p_tagSize,
                                             void const*    p_tag) noexcept
{
    #ifndef NDEBUG

    VkDebugUtilsObjectTagInfoEXT tagInfo = { VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_TAG_INFO_EXT };

    tagInfo.objectType   = p_objectType;
    tagInfo.objectHandle = p_objectHandle;
    tagInfo.tagName      = p_name;
    tagInfo.tagSize      = p_tagSize;
    tagInfo.pTag         = p_tag;

    VK_CHECK_RESULT(vkSetDebugUtilsObjectTagEXT(p_device, &tagInfo));

    #endif
}

void    Debug::BeginQueueLabelRegion        (VkQueue  const  p_queue,
                                             ANSICHAR const* p_labelName,
                                             Color    const& p_color) noexcept
{
    #ifndef NDEBUG

    VkDebugUtilsLabelEXT label = { VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT };

    label.pLabelName = p_labelName;

    for(int32 i = 0; i < 4; ++i)
        label.color[i] = p_color[i];

    vkQueueBeginDebugUtilsLabelEXT(p_queue, &label);

    #endif
}

void    Debug::EndQueueLabelRegion          (VkQueue const p_queue) noexcept
{
    #ifndef NDEBUG

    vkQueueEndDebugUtilsLabelEXT(p_queue);
    
    #endif
}

void    Debug::InsertQueueLabel             (VkQueue  const  p_queue,
                                             ANSICHAR const* p_labelName,
                                             Color    const& p_color) noexcept
{
    #ifndef NDEBUG

    VkDebugUtilsLabelEXT label = { VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT };

    label.pLabelName = p_labelName;

    for(int32 i = 0; i < 4; ++i)
        label.color[i] = p_color[i];

    vkQueueInsertDebugUtilsLabelEXT(p_queue, &label);

    #endif
}

void    Debug::BeginCmdBufferLabelRegion    (VkCommandBuffer const  p_cmdBuffer,
                                             ANSICHAR        const* p_labelName,
                                             Color           const& p_color) noexcept
{
    #ifndef NDEBUG

    VkDebugUtilsLabelEXT label = { VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT };

    label.pLabelName = p_labelName;

    for(int32 i = 0; i < 4; ++i)
        label.color[i] = p_color[i];

    vkCmdBeginDebugUtilsLabelEXT(p_cmdBuffer, &label);

    #endif
}

void    Debug::EndCmdBufferLabelRegion      (VkCommandBuffer const p_cmdBuffer) noexcept
{
    #ifndef NDEBUG

    vkCmdEndDebugUtilsLabelEXT(p_cmdBuffer);

    #endif
}

void    Debug::InsertCmdBufferLabel         (VkCommandBuffer const  p_cmdBuffer,
                                             ANSICHAR        const* p_labelName,
                                             Color           const& p_color) noexcept
{
    #ifndef NDEBUG

    VkDebugUtilsLabelEXT label = { VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT };

    label.pLabelName = p_labelName;

    for(int32 i = 0; i < 4; ++i)
        label.color[i] = p_color[i];

    vkCmdInsertDebugUtilsLabelEXT(p_cmdBuffer, &label);

    #endif
}