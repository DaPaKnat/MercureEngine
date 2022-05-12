#include "PCH.hpp"
#include "RHI.hpp"

#include "Vulkan/RenderPasses/RenderPass.hpp"

// ============================== [Destructor] ============================== //

RenderPass::RenderPass  () :
    m_handle { VK_NULL_HANDLE }
{

}

RenderPass::~RenderPass ()
{
    vkDestroyRenderPass(RHI::Get().GetDevice()->GetLogicalDevice(), m_handle, nullptr);
}