#include "PCH.hpp"
#include "RHI.hpp"

#include "Vulkan/Object/Fence.hpp"

// ============================== [Public Constructor and Destructor] ============================== //

Fence::Fence    (VkFenceCreateFlags p_flags)
{
    VkFenceCreateInfo fenceCI = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };

    fenceCI.flags = p_flags;

    VK_CHECK_RESULT(vkCreateFence(RHI::Get().GetDevice()->GetLogicalDevice(), &fenceCI, nullptr, &m_handle));
}

Fence::~Fence   ()
{
    vkDestroyFence(RHI::Get().GetDevice()->GetLogicalDevice(), m_handle, nullptr);
}

// ============================== [Public Local Methods] ============================== //

void    Fence::Reset    () const noexcept
{
    VK_CHECK_RESULT(vkResetFences(RHI::Get().GetDevice()->GetLogicalDevice(), 1u, &m_handle));
}

void    Fence::Wait     () const noexcept
{
    VK_CHECK_RESULT(vkWaitForFences(RHI::Get().GetDevice()->GetLogicalDevice(), 1u, &m_handle, VK_TRUE, MAX_UINT_64));
}