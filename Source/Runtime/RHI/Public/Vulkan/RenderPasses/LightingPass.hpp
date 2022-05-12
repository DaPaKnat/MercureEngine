#ifndef __VULKAN_LIGHTING_PASS_HPP__
#define __VULKAN_LIGHTING_PASS_HPP__

#include "RenderPass.hpp"

class ENGINE_API LightingPass : public RenderPass
{
    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        LightingPass    () = delete;

        LightingPass    (std::vector<Frame> const& p_frames);

        ~LightingPass   ();

    // ============================== [Public Local Methods] ============================== //

        void    Rebuild (std::vector<Frame> const&  p_frames)   noexcept final override;

        void    Draw    (Frame              const&  p_frame)    noexcept final override;

    // ==================================================================================== //

        INLINE Image const& GetPositionImage    (size_t p_index)    const noexcept  { return m_attachments[p_index].position; }
        
        INLINE Image const& GetNormalImage      (size_t p_index)    const noexcept  { return m_attachments[p_index].normal; }

        INLINE Image const& GetResult           (size_t p_index)    const noexcept  { return m_attachments[p_index].composition; }

    protected:

    // ============================== [Data Structure] ============================== //

        struct Attachment
        {
            Image           depth;
            Image           position;
            Image           normal;
            Image           albedo;
            Image           composition;
            VkFramebuffer   framebuffer;
            VkDescriptorSet descriptorSet;
        };

    // ============================== [Protected Local Properties] ============================== //

        std::vector<Attachment> m_attachments;

        VkDescriptorPool        m_descriptorPool;

        VkDescriptorSetLayout   m_descriptorSetLayout;

        VkPipelineLayout        m_pipelineLayout;

        VkPipeline              m_pipeline;

    // ============================== [Protected Local Methods] ============================== //

        void    SetupRenderPass     (std::vector<Frame> const& p_frames) noexcept final override;

        void    SetupFramebuffers   (std::vector<Frame> const& p_frames) noexcept final override;

        void    SetupPipelines      (std::vector<Frame> const& p_frames) noexcept final override;

    // ======================================================================================= //

        void    SetupCompositionDescriptorPool      (std::vector<Frame> const& p_frames) noexcept;

        void    SetupCompositionDescriptorSetLayout (std::vector<Frame> const& p_frames) noexcept;

        void    SetupCompositionDescriptorSets      (std::vector<Frame> const& p_frames) noexcept;

        void    SetupCompositionPipelineLayout      (std::vector<Frame> const& p_frames) noexcept;

        void    SetupCompositionPipeline            (std::vector<Frame> const& p_frames) noexcept;

    // ======================================================================================= //
        
        void    GBufferPass     (Frame const& p_frame) const noexcept;

        void    CompositionPass (Frame const& p_frame) const noexcept;

        void    TransparentPass (Frame const& p_frame) const noexcept;

};  // !class RenderPass

#endif // !__VULKAN_LIGHTING_PASS_HPP__