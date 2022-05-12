#ifndef __VULKAN_INSTANCE_HPP__
#define __VULKAN_INSTANCE_HPP__

#include "Vulkan/Vulkan.hpp"

class ENGINE_API Instance : public UniqueObject
{
    public:

    // ============================== [Public Constructor and Destructor] ============================== //

        Instance    ();

        ~Instance   ();

    // ============================== [Public Local Methods] ============================== //

        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE VkInstance const GetHandle() const noexcept { return m_instance; }

    private:

    // ============================== [Private Local Properties] ============================== //

        VkInstance                      m_instance;

        std::vector<ANSICHAR const*>    m_requiredExtensions;

        std::vector<ANSICHAR const*>    m_requiredLayers;

    // ============================== [Private Local Methods] ============================== //

        void    CheckInstanceExtensions ()  const noexcept;

        void    CheckInstanceLayers     ()  const noexcept;

        void    CreateInstance          ()  noexcept;

};  // !class Instance

#endif // !__VULKAN_INSTANCE_HPP__