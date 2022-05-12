#ifndef __VULKAN_RENDER_PASS_HPP__
#define __VULKAN_RENDER_PASS_HPP__

#include "Vulkan/Vulkan.hpp"

// ============================== [Forward Declaration] ============================== //

struct Frame;

// ============================== [Global Enum] ============================== //

enum class ERenderStage : uint8
{
    SHADOW      = (1u << 0),
    LIGHTING    = (1u << 1),
    BLOOM       = (1u << 2),
    TONEMAPPING = (1u << 3)

};  // !enum class ERenderStage

// =========================================================================== //

/**
 * Base class for any renderpass object.
 */
class ENGINE_API BASE RenderPass : public UniqueObject
{
    public:

    // ============================== [Public Constructor and Destructor] ============================== //

        RenderPass          ();

        virtual ~RenderPass ();

    // ============================== [Public Local Methods] ============================== //

        virtual void    Rebuild (std::vector<Frame> const&  p_frames)   = 0;

        virtual void    Draw    (Frame              const&  p_frame)    = 0;

    // ==================================================================================== //

        /**
         * @thread_safety This function may be called from any thread.
         */
        INLINE VkRenderPass const GetHandle() const noexcept { return m_handle; }

    protected:

    // ============================== [Protected Local Properties] ============================== //

        VkRenderPass m_handle;

    // ============================== [Protected Local Methods] ============================== //

        virtual void    SetupRenderPass     (std::vector<Frame> const& p_frames) = 0;

        virtual void    SetupFramebuffers   (std::vector<Frame> const& p_frames) = 0;

        virtual void    SetupPipelines      (std::vector<Frame> const& p_frames) = 0;

};  // !class RenderPass

#endif // !__VULKAN_RENDER_PASS_HPP__