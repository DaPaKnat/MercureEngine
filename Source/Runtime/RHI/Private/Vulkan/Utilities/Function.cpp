#include "PCH.hpp"

#include "Vulkan/Utilities/Loader.hpp"
#include "Vulkan/Utilities/Debug.hpp"
#include "Vulkan/Utilities/Function.hpp"

// ============================== [Global Function Pointers] ============================== //

/** Function used to create a debug messenger object. */
PFN_vkCreateDebugUtilsMessengerEXT  CreateDebugUtilsMessengerEXT    = nullptr;

/** Function used to destroy a debug messenger object. */
PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessengerEXT   = nullptr;

/** Function used to give a user-friendly name to an object. */
PFN_vkSetDebugUtilsObjectNameEXT    SetDebugUtilsObjectNameEXT      = nullptr;

/** Function used to attach arbitrary data to an object. */
PFN_vkSetDebugUtilsObjectTagEXT     SetDebugUtilsObjectTagEXT       = nullptr;

/** Function used to open a queue debug label region. */
PFN_vkQueueBeginDebugUtilsLabelEXT  QueueBeginDebugUtilsLabelEXT    = nullptr;

/** Function used to close a queue debug label region. */
PFN_vkQueueEndDebugUtilsLabelEXT    QueueEndDebugUtilsLabelEXT      = nullptr;

/** Function used to insert a label into a queue. */
PFN_vkQueueInsertDebugUtilsLabelEXT QueueInsertDebugUtilsLabelEXT   = nullptr;

/** Function used to open a command buffer debug label region. */
PFN_vkCmdBeginDebugUtilsLabelEXT    CmdBeginDebugUtilsLabelEXT      = nullptr;

/** Function used to close a command buffer label region. */
PFN_vkCmdEndDebugUtilsLabelEXT      CmdEndDebugUtilsLabelEXT        = nullptr;

/** Function used to insert a label into a command buffer. */
PFN_vkCmdInsertDebugUtilsLabelEXT   CmdInsertDebugUtilsLabelEXT     = nullptr;

// ============================== [Vulkan Methods Implementation] ============================== //

/**
 * Creates a debug messenger object.
 *
 * @param p_instance    Opaque handle to the instance object.
 * @param p_createInfo  Structure specifying parameters of a newly created debug messenger.
 * @param p_allocator   Structure containing callback function pointers for memory allocation.
 * @param p_messenger   Pointer to record the debug messenger object created.
 *
 * @return              Whether or not the debug messenger object could be created.
 */
VKAPI_ATTR VkResult VKAPI_CALL  vkCreateDebugUtilsMessengerEXT  (VkInstance                         const  p_instance,
                                                                 VkDebugUtilsMessengerCreateInfoEXT const* p_createInfo, 
                                                                 VkAllocationCallbacks              const* p_allocator, 
                                                                 VkDebugUtilsMessengerEXT*                 p_messenger)
{
    if (CreateDebugUtilsMessengerEXT)
        return CreateDebugUtilsMessengerEXT(p_instance, p_createInfo, p_allocator, p_messenger);
        
    LOG(LogRHI, Error, "vkCreateDebugUtilsMessengerEXT is not loaded");

    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

/**
 * Destroys a debug messenger object.
 *
 * @param p_instance    Opaque handle to the instance object.
 * @param p_messenger   Opaque handle to the debug messenger object.
 * @param p_allocator   Structure containing callback function pointers for memory allocation.
 */
VKAPI_ATTR void     VKAPI_CALL  vkDestroyDebugUtilsMessengerEXT (VkInstance               const  p_instance,
                                                                 VkDebugUtilsMessengerEXT const  p_messenger,
                                                                 VkAllocationCallbacks    const* p_allocator) 
{ 
    if (DestroyDebugUtilsMessengerEXT)
        DestroyDebugUtilsMessengerEXT(p_instance, p_messenger, p_allocator);
    else
        LOG(LogRHI, Error, "vkDestroyDebugUtilsMessengerEXT is not loaded");

    return;
}

/**
 * Give a user-friendly name to an object.
 *
 * @param p_device      Opaque handle to the device that created the object.
 * @parma p_nameInfo    Structure specifying the parameters of the name to set on the object.
 *
 * @return              Whether or not a user-friendly name could be given to an object.
 */
VKAPI_ATTR VkResult VKAPI_CALL  vkSetDebugUtilsObjectNameEXT    (VkDevice                      const  p_device,
                                                                 VkDebugUtilsObjectNameInfoEXT const* p_nameInfo)
{
    if (SetDebugUtilsObjectNameEXT)
        return SetDebugUtilsObjectNameEXT(p_device, p_nameInfo);

    LOG(LogRHI, Error, "vkSetDebugUtilsObjectNameEXT is not loaded");

    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

/**
 * Attaches arbitrary data to an object.
 *
 * @param p_device  Opaque handle to the device that created the object.
 * @param p_tagInfo Structure specifying the parameters of the tag to attach to the object.
 *
 * @return          Whether or not arbitrary data could be attached to an object.
 */
VKAPI_ATTR VkResult VKAPI_CALL  vkSetDebugUtilsObjectTagEXT     (VkDevice                     const  p_device,
                                                                 VkDebugUtilsObjectTagInfoEXT const* p_tagInfo)
{
    if (SetDebugUtilsObjectTagEXT)
        return SetDebugUtilsObjectTagEXT(p_device, p_tagInfo);

    LOG(LogRHI, Error, "vkSetDebugUtilsObjectTagEXT is not loaded");

    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

/**
 * Opens a queue debug label region.
 *
 * @param p_queue       Opaque handle to the queue in which to start a debug label region.
 * @param p_labelInfo   Structure specifying the parameters of the label region to open.
 */
VKAPI_ATTR void     VKAPI_CALL  vkQueueBeginDebugUtilsLabelEXT  (VkQueue              const  p_queue,
                                                                 VkDebugUtilsLabelEXT const* p_labelInfo)
{
    if (QueueBeginDebugUtilsLabelEXT)
        QueueBeginDebugUtilsLabelEXT(p_queue, p_labelInfo);
    else
        LOG(LogRHI, Error, "vkQueueBeginDebugUtilsLabelEXT is not loaded");

    return;
}

/**
 * Closes a queue debug label region.
 *
 * @param p_queue   Opaque handle to the queue in which a debug label region should be closed..
 */
VKAPI_ATTR void     VKAPI_CALL  vkQueueEndDebugUtilsLabelEXT    (VkQueue const p_queue)
{
    if (QueueEndDebugUtilsLabelEXT)
        QueueEndDebugUtilsLabelEXT(p_queue);
    else
        LOG(LogRHI, Error, "vkQueueEndDebugUtilsLabelEXT is not loaded");

    return;
}

/**
 * Inserts a label into a queue.
 *
 * @param p_queue       Opaque handle to the queue into which a debug label will be inserted..
 * @param p_labelInfo   Structure specifying the parameters of the label to insert.
 */
VKAPI_ATTR void     VKAPI_CALL  vkQueueInsertDebugUtilsLabelEXT (VkQueue              const  p_queue,
                                                                 VkDebugUtilsLabelEXT const* p_labelInfo)
{
    if (QueueInsertDebugUtilsLabelEXT)
        QueueInsertDebugUtilsLabelEXT(p_queue, p_labelInfo);
    else
        LOG(LogRHI, Error, "vkQueueInsertDebugUtilsLabelEXT is not loaded");

    return;
}

/**
 * Opens a command buffer debug label region.
 *
 * @param p_commandBuffer   Opaque handle to the command buffer into which the command is recorded.
 * @param p_labelInfo       Structure specifying the parameters of the label region to open.
 */
VKAPI_ATTR void     VKAPI_CALL  vkCmdBeginDebugUtilsLabelEXT    (VkCommandBuffer      const  p_commandBuffer,
                                                                 VkDebugUtilsLabelEXT const* p_labelInfo)
{
    if (CmdBeginDebugUtilsLabelEXT)
        CmdBeginDebugUtilsLabelEXT(p_commandBuffer, p_labelInfo);
    else
        LOG(LogRHI, Error, "vkCmdBeginDebugUtilsLabelEXT is not loaded");

    return;
}

/**
 * Closes a command buffer label region.
 *
 * @param p_commandBuffer   Opaque handle to the command buffer into which the command is recorded.
 */
VKAPI_ATTR void     VKAPI_CALL  vkCmdEndDebugUtilsLabelEXT      (VkCommandBuffer const p_commandBuffer)
{
    if (CmdEndDebugUtilsLabelEXT)
        CmdEndDebugUtilsLabelEXT(p_commandBuffer);
    else
        LOG(LogRHI, Error, "vkCmdEndDebugUtilsLabelEXT is not loaded");

    return;
}

/**
 * Inserts a label into a command buffer.
 *
 * @param p_commandBuffer   Opaque handle to the command buffer into which the command is recorded.
 * @param p_labelInfo       Structure specifying the parameters of the label to insert.
 */
VKAPI_ATTR void     VKAPI_CALL  vkCmdInsertDebugUtilsLabelEXT   (VkCommandBuffer      const  p_commandBuffer,
                                                                 VkDebugUtilsLabelEXT const* p_labelInfo)
{
    if (CmdInsertDebugUtilsLabelEXT)
        CmdInsertDebugUtilsLabelEXT(p_commandBuffer, p_labelInfo);
    else
        LOG(LogRHI, Error, "vkCmdInsertDebugUtilsLabelEXT is not loaded");

    return;
}

// ============================== [Vulkan Style Methods] ============================== //

VKAPI_ATTR VkBool32 VKAPI_CALL  vkDebugCallback                     (VkDebugUtilsMessageSeverityFlagBitsEXT      p_messageSeverity,
                                                                     VkDebugUtilsMessageTypeFlagsEXT             p_messageType,
                                                                     VkDebugUtilsMessengerCallbackDataEXT const* p_callbackData,
                                                                     void*                                       p_userData)
{
    std::string message;

    // Checks which types of events caused the debug callback.
    if (p_messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
        message += " | GENERAL";
    if (p_messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
        message += " | VALIDATION";
    if (p_messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
        message += " | PERFORMANCE";

    // Deletes the first two spaces and vertical bar for readability.
    message.erase(0, 3);
    
    message += " - ID Name : ";
    message += p_callbackData->pMessageIdName ? p_callbackData->pMessageIdName : "";

    message += " | ID Number : ";
    message += p_callbackData->messageIdNumber;
    
    message += "\n\n";
    message += p_callbackData->pMessage;

    ANSICHAR tmp[2048];

    // Checks which object(s) caused a debug callback.
    if (p_callbackData->objectCount > 0u)
    {
        sprintf_s(tmp, "\nObjects - %d :\n", p_callbackData->objectCount);

        message += tmp;

        // The most recent label should be the first one.
        for (uint32 i = 0; i < p_callbackData->objectCount; ++i)
        {
            sprintf_s(tmp, "\tObject[%d] - %s, Handle = %p, Name = \"%s\"\n", i, Debug::ToString(p_callbackData->pObjects[i].objectType),
                                                                                 (void*)p_callbackData->pObjects[i].objectHandle,
                                                                                  p_callbackData->pObjects[i].pObjectName);

            message += tmp;
        }
    }

    // Checks what are the last command buffer labels encountered before the debug callback.
    if (p_callbackData->cmdBufLabelCount > 0u)
    {
        sprintf_s(tmp, "\nCommand Buffer Labels - %d :\n", p_callbackData->cmdBufLabelCount);

        message += tmp;

        // The most recent label should be the first one.
        for (uint32 i = 0; i < p_callbackData->cmdBufLabelCount; ++i)
        {
            sprintf_s(tmp, "\tLabel[%d] - Name = %s\n", i, p_callbackData->pCmdBufLabels[i].pLabelName);

            message += tmp;
        }
    }

    // Checks what are the last queue labels encountered before the debug callback.
    if (p_callbackData->queueLabelCount > 0u)
    {
        sprintf_s(tmp, "\nQueue Labels - %d :\n", p_callbackData->queueLabelCount);

        message += tmp;

        // The most recent label should be the first one.
        for (uint32 i = 0; i < p_callbackData->queueLabelCount; ++i)
        {
            sprintf_s(tmp, "\tLabel[%d] - Name = %s\n", i, p_callbackData->pQueueLabels[i].pLabelName);

            message += tmp;
        }
    }

    // Checks which severity of events caused a debug callback.
    switch (p_messageSeverity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            LOG(LogRHI, Log, message.c_str());
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            LOG(LogRHI, Log, message.c_str());
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            LOG(LogRHI, Warning, message.c_str());
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            LOG(LogRHI, Error, message.c_str());
            break;

        default:
            LOG(LogRHI, Display, message.c_str());
            break;
    }

    return VK_FALSE;
}

VKAPI_ATTR void*    VKAPI_CALL  vkAllocate                          (void*                   p_userData,
                                                                     size_t                  p_size,
                                                                     size_t                  p_alignment,
                                                                     VkSystemAllocationScope p_allocationScope)
{
    return nullptr;
}

VKAPI_ATTR void*    VKAPI_CALL  vkReallocate                        (void*                   p_userData,
                                                                     void*                   p_original,
                                                                     size_t                  p_size,
                                                                     size_t                  p_alignment,
                                                                     VkSystemAllocationScope p_allocationScope)
{
    return nullptr;
}

VKAPI_ATTR void     VKAPI_CALL  vkFree                              (void* p_userData,
                                                                     void* p_memory)
{
    return;
}

VKAPI_ATTR void     VKAPI_CALL  vkInternalAllocationNotification    (void*                    p_userData,
                                                                     size_t                   p_size,
                                                                     VkInternalAllocationType p_allocationType,
                                                                     VkSystemAllocationScope  p_allocationScope)
{
    return;
}

VKAPI_ATTR void     VKAPI_CALL  vkInternalFreeNotification          (void*                    p_userData,
                                                                     size_t                   p_size,
                                                                     VkInternalAllocationType p_allocationType,
                                                                     VkSystemAllocationScope  p_allocationScope)
{
    return;
}