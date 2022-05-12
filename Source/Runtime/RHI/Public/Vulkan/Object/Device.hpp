#ifndef __VULKAN_DEVICE_HPP__
#define __VULKAN_DEVICE_HPP__

#include "Vulkan/Object/Queue.hpp"
#include "Vulkan/Object/CommandPool.hpp"

// ============================== [Data Structures] ============================== //

struct QueueFamilies
{
    uint32 graphicsFamily = MAX_UINT_32;
    uint32 presentFamily  = MAX_UINT_32;
    uint32 computeFamily  = MAX_UINT_32;
    uint32 transferFamily = MAX_UINT_32;

};  // !struct QueueFamilies

struct Queues
{
    std::unique_ptr<Queue> graphicsQueue = nullptr;
    std::unique_ptr<Queue> presentQueue  = nullptr;
    std::unique_ptr<Queue> computeQueue  = nullptr;
    std::unique_ptr<Queue> transferQueue = nullptr;

};  // !struct Queues

struct CommandPools
{
    std::unordered_map<std::thread::id, std::unique_ptr<CommandPool>> graphicsCmdPools;
    std::unordered_map<std::thread::id, std::unique_ptr<CommandPool>> computeCmdPools;
    std::unordered_map<std::thread::id, std::unique_ptr<CommandPool>> transferCmdPools;

};  // !struct CommandPools

// =============================================================================== //

class ENGINE_API Device : public UniqueObject
{
    public:

    // ============================== [Public Constructor and Destructor] ============================== //

        Device  ();

        ~Device ();

    // ============================== [Public Local Methods] ============================== //

        /**
         * @thread_safety This function may be called from any thread.
         */
        VkFormat                FindSupportedFormat     (std::vector<VkFormat> const&   p_candidates,
                                                         VkImageTiling                  p_tiling,
                                                         VkFormatFeatureFlags           p_features) const noexcept;

        VkSampleCountFlagBits   GetMaxUsableSampleCount ()                                          const noexcept;

        /**
         * @thread_safety This function must only be called from the main thread.
         */
        void                    WaitIdle                ()                                          const noexcept;

    // ==================================================================================== //

        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE VkPhysicalDevice                 const   GetPhysicalDevice       ()  const noexcept
        { 
            return m_physicalDevice;
        }

        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE VkDevice                         const   GetLogicalDevice        ()  const noexcept
        { 
            return m_device;
        }
        
        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE VkPhysicalDeviceProperties       const&  GetProperties           ()  const noexcept
        { 
            return m_properties;
        };

        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE VkPhysicalDeviceFeatures         const&  GetFeatures             ()  const noexcept
        { 
            return m_features;
        }

        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE VkPhysicalDeviceMemoryProperties const&  GetMemoryProperties     ()  const noexcept
        { 
            return m_memoryProperties;
        }

        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE uint32                                   GetGraphicsFamily       ()  const noexcept
        {
            return m_queueFamilies.graphicsFamily;
        }
        
        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE uint32                                   GetPresentFamily        ()  const noexcept
        {
            return m_queueFamilies.presentFamily;
        }
        
        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE uint32                                   GetComputeFamily        ()  const noexcept
        {
            return m_queueFamilies.computeFamily;
        }
        
        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE uint32                                   GetTransferFamily       ()  const noexcept
        {
            return m_queueFamilies.transferFamily;
        }
        
        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE std::unique_ptr<Queue>       const&      GetGraphicsQueue        ()  const noexcept
        {
            return m_queues.graphicsQueue;
        }
        
        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE std::unique_ptr<Queue>       const&      GetPresentQueue         ()  const noexcept
        {
            return m_queues.presentQueue;
        }
        
        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE std::unique_ptr<Queue>       const&      GetComputeQueue         ()  const noexcept
        {
            return m_queues.computeQueue;
        }
        
        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE std::unique_ptr<Queue>       const&      GetTransferQueue        ()  const noexcept
        {
            return m_queues.transferQueue;
        }
        
        /**
         * @thread_safety This function may be called from any thread created by the ThreadPool.
         */
        INLINE std::unique_ptr<CommandPool> const&      GetGraphicsCommandPool  ()  const noexcept
        {
            return m_cmdPools.graphicsCmdPools.at(std::this_thread::get_id());
        }
        
        /**
         * @thread_safety This function may be called from any thread created by the ThreadPool.
         */
        INLINE std::unique_ptr<CommandPool> const&      GetComputeCommandPool   ()   const noexcept
        {
            return m_cmdPools.computeCmdPools.at(std::this_thread::get_id());
        }
        
        /**
         * @thread_safety This function may be called from any thread created by the ThreadPool.
         */
        INLINE std::unique_ptr<CommandPool> const&      GetTransferCommandPool  ()  const noexcept
        {
            return m_cmdPools.transferCmdPools.at(std::this_thread::get_id());
        }

    private:

    // ============================== [Private Local Properties] ============================== //

        VkPhysicalDevice                    m_physicalDevice;

        std::vector<ANSICHAR const*>        m_requiredExtensions;

        VkPhysicalDeviceProperties          m_properties;

        VkPhysicalDeviceFeatures            m_features;

        VkPhysicalDeviceMemoryProperties    m_memoryProperties;

    // ======================================================================================== //

        VkDevice        m_device;

        QueueFamilies   m_queueFamilies;

        Queues          m_queues;

        CommandPools    m_cmdPools;

    // ============================== [Private Local Methods] ============================== //

        void    PickPhysicalDevice          ()                                  noexcept;

        uint32  RateDeviceSuitability       (VkPhysicalDevice const p_device)   const noexcept;

        bool    CheckDeviceExtensions       (VkPhysicalDevice const p_device)   noexcept;

        void    EnumerateDeviceProperties   ()                                  noexcept;
    
    // ===================================================================================== //

        void    FindQueueFamilies   ()  noexcept;

        void    CreateLogicalDevice ()  noexcept;

        void    CreateQueues        ()  noexcept;

        void    CreateCommandPools  ()  noexcept;

};  // !class Device

#endif // !__VULKAN_DEVICE_HPP__