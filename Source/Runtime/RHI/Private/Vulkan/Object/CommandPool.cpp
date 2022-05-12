#include "PCH.hpp"
#include "RHI.hpp"

#include "Vulkan/Object/CommandPool.hpp"

// ============================== [Public Constructor] ============================== //

CommandPool::CommandPool    (uint32        p_queueFamily,
                             VkCommandPool p_handle) :
    m_queueFamily { p_queueFamily },
    m_handle      { p_handle }
{

}

// ============================== [Public Local Methods] ============================== //

CommandBuffer   CommandPool::AllocateCommandBuffer  (VkCommandBufferLevel p_cmdBufferLevel) const noexcept
{
    VkCommandBuffer             cmdBuffer   = VK_NULL_HANDLE;
    VkCommandBufferAllocateInfo cmdBufferAI = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };

    cmdBufferAI.commandPool        = m_handle;
    cmdBufferAI.level              = p_cmdBufferLevel;
    cmdBufferAI.commandBufferCount = 1u;

    VK_CHECK_RESULT(vkAllocateCommandBuffers(RHI::Get().GetDevice()->GetLogicalDevice(), &cmdBufferAI, &cmdBuffer));

    return CommandBuffer(cmdBuffer);
}


void            CommandPool::FreeCommandBuffer      (CommandBuffer const& p_cmdBuffer) const noexcept
{
    VkCommandBuffer const cmdBuffer = p_cmdBuffer.GetHandle();

    vkFreeCommandBuffers(RHI::Get().GetDevice()->GetLogicalDevice(), m_handle, 1u, &cmdBuffer);
}