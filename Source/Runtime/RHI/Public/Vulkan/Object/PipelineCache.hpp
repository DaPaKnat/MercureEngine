#ifndef __VULKAN_PIPELINE_CACHE_HPP__
#define __VULKAN_PIPELINE_CACHE_HPP__

#include "Vulkan/Vulkan.hpp"

class ENGINE_API PipelineCache : public UniqueObject
{
    public:

    // ============================== [Public Constructor and Destructor] ============================== //

        PipelineCache   ();

        ~PipelineCache  ();
    
    // ============================== [Public Local Methods] ============================== //

        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE VkPipelineCache const GetHandle() const noexcept { return m_handle; }

    private:

    // ============================== [Private Local Properties] ============================== //

        VkPipelineCache m_handle;

};  // !class PipelineCache

#endif // !__VULKAN_PIPELINE_CACHE_HPP__