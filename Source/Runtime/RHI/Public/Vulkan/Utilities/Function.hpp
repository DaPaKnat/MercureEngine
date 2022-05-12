#ifndef __VULKAN_FUNCTION_HPP__
#define __VULKAN_FUNCTION_HPP__

#include "CoreMinimal.hpp" 

// ============================== [Global Function Pointers] ============================== //

/** Function used to create a debug messenger object. */
extern PFN_vkCreateDebugUtilsMessengerEXT   CreateDebugUtilsMessengerEXT;

/** Function used to destroy a debug messenger object. */
extern PFN_vkDestroyDebugUtilsMessengerEXT  DestroyDebugUtilsMessengerEXT;

/** Function used to give a user-friendly name to an object. */
extern PFN_vkSetDebugUtilsObjectNameEXT     SetDebugUtilsObjectNameEXT;

/** Function used to attach arbitrary data to an object. */
extern PFN_vkSetDebugUtilsObjectTagEXT      SetDebugUtilsObjectTagEXT;

/** Function used to open a queue debug label region. */
extern PFN_vkQueueBeginDebugUtilsLabelEXT   QueueBeginDebugUtilsLabelEXT;

/** Function used to close a queue debug label region. */
extern PFN_vkQueueEndDebugUtilsLabelEXT     QueueEndDebugUtilsLabelEXT;

/** Function used to insert a label into a queue. */
extern PFN_vkQueueInsertDebugUtilsLabelEXT  QueueInsertDebugUtilsLabelEXT;

/** Function used to open a command buffer debug label region. */
extern PFN_vkCmdBeginDebugUtilsLabelEXT     CmdBeginDebugUtilsLabelEXT;

/** Function used to close a command buffer label region. */
extern PFN_vkCmdEndDebugUtilsLabelEXT       CmdEndDebugUtilsLabelEXT;

/** Function used to insert a label into a command buffer. */
extern PFN_vkCmdInsertDebugUtilsLabelEXT    CmdInsertDebugUtilsLabelEXT;

// ============================== [Vulkan Style Methods] ============================== //

/**
 * Application-defined debug messenger callback function.
 *
 * @param p_messageSeverity Bitmask specifying which severities of events cause a debug messenger callback.
 * @param p_messageType     Bitmask specifying which types of events cause a debug messenger callback.
 * @param p_callbackData    Structure specifying parameters returned to the callback.
 * @param p_userData        User data provided when the messenger object was created.
 *
 * @return                  Always "VK_FALSE" because we don't want the API to abort.
 */
VKAPI_ATTR VkBool32 VKAPI_CALL  vkDebugCallback                     (VkDebugUtilsMessageSeverityFlagBitsEXT         p_messageSeverity,
                                                                     VkDebugUtilsMessageTypeFlagsEXT                p_messageType,
                                                                     VkDebugUtilsMessengerCallbackDataEXT const*    p_callbackData,
                                                                     void*                                          p_userData);

/**
 * Application-defined memory allocation function.
 *
 * @param p_userData        Value specified in the allocator specified by the application.
 * @param p_size            Size in bytes of the requested allocation.
 * @param p_alignment       Requested alignment of the allocation in bytes and must be a power of two.
 * @param p_allocationScope Value specifying the allocation scope of the lifetime of the allocation.
 *
 * @return                  Pointer to memory allocation containing at least "p_size" bytes, and with the pointer value being a multiple of "p_alignment".
 */
VKAPI_ATTR void*    VKAPI_CALL  vkAllocate                          (void*                                          p_userData,
                                                                     size_t                                         p_size,
                                                                     size_t                                         p_alignment,
                                                                     VkSystemAllocationScope                        p_allocationScope);

/**
 * Application-defined memory reallocation function.
 *
 * @param p_userData        Value specified in the allocator specified by the application.
 * @param p_original        Pointer previously returned by the "allocate" function of the same allocator.
 * @param p_size            Size in bytes of the requested allocation.
 * @param p_alignment       Requested alignment of the allocation in bytes and must be a power of two.
 * @param p_allocationScope Value specifying the allocation scope of the lifetime of the allocation.
 *
 * @return                  Pointer to memory allocation containing at least "p_size" bytes, and with the pointer value being a multiple of "p_alignment".
 */
VKAPI_ATTR void*    VKAPI_CALL  vkReallocate                        (void*                                          p_userData,
                                                                     void*                                          p_original,
                                                                     size_t                                         p_size,
                                                                     size_t                                         p_alignment,
                                                                     VkSystemAllocationScope                        p_allocationScope);

/**
 * Application-defined memory free function.
 *
 * @param p_userData    Value specified in the allocator specified by the application.
 * @param p_memory      Allocation to be freed.
 */
VKAPI_ATTR void     VKAPI_CALL  vkFree                              (void*                                          p_userData,
                                                                     void*                                          p_memory);

/**
 * Application-defined memory allocation notification function.
 *
 * @param p_userData        Value specified in the allocator specified by the application.
 * @param p_size            Requested size of an allocation.
 * @param p_allocationType  Value specifying the requested type of an allocation.
 * @param p_allocationScope Value specifying the allocation scope of the lifetime of the allocation.
 */
VKAPI_ATTR void     VKAPI_CALL  vkInternalAllocationNotification    (void*                                          p_userData,
                                                                     size_t                                         p_size,
                                                                     VkInternalAllocationType                       p_allocationType,
                                                                     VkSystemAllocationScope                        p_allocationScope);

/**
 * Application-defined memory free notification function.
 *
 * @param p_userData        Value specified in the allocator specified by the application.
 * @param p_size            Requested size of an allocation.
 * @param p_allocationType  Value specifying the requested type of an allocation.
 * @param p_allocationScope Value specifying the allocation scope of the lifetime of the allocation.
 */
VKAPI_ATTR void     VKAPI_CALL  vkInternalFreeNotification          (void*                                          p_userData,
                                                                     size_t                                         p_size,
                                                                     VkInternalAllocationType                       p_allocationType,
                                                                     VkSystemAllocationScope                        p_allocationScope);

#endif // !__VULKAN_FUNCTION_HPP__