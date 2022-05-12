#ifndef __VULKAN_SHADOW_PASS_HPP__
#define __VULKAN_SHADOW_PASS_HPP__

#include "RenderPass.hpp"

class ENGINE_API ShadowPass : public RenderPass
{
    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        ShadowPass  () = delete;

        ShadowPass  (std::vector<Frame> const& p_frames) noexcept;

        ~ShadowPass ();

    // ============================== [Public Local Methods] ============================== //

        void    Rebuild (std::vector<Frame> const&  p_frames)   noexcept final override;

        void    Draw    (Frame              const&  p_frame)    noexcept final override;

    // ==================================================================================== //

        INLINE Image     const& GetShadow2D         (size_t p_index)    const noexcept  { return m_attachments[p_index].shadow2D; }

        INLINE Image     const& GetShadowCube       (size_t p_index)    const noexcept  { return m_attachments[p_index].shadowCube; }

        INLINE VkSampler const  GetShadowSampler    ()                  const noexcept  { return m_sampler; }

    protected:

    // ============================== [Data Structure] ============================== //

        struct Attachment
        {
            Image           shadow2D;
            Image           shadowCube;
            VkFramebuffer   shadow2DFramebuffer;
            VkFramebuffer   shadowCubeFramebuffer;
            VkDescriptorSet descriptorSet;
        };

    // ============================== [Protected Local Properties] ============================== //

        std::vector<Attachment> m_attachments;

        VkSampler               m_sampler;

        VkDescriptorPool        m_descriptorPool;

        VkDescriptorSetLayout   m_descriptorSetLayout;

        VkPipelineLayout        m_pipelineLayout;

        VkPipeline              m_shadow2DPipeline;

        VkPipeline              m_shadowCubePipeline;

    // ============================== [Protected Local Methods] ============================== //

        void    SetupRenderPass     (std::vector<Frame> const& p_frames) noexcept final override;

        void    SetupFramebuffers   (std::vector<Frame> const& p_frames) noexcept final override;

        void    SetupPipelines      (std::vector<Frame> const& p_frames) noexcept final override;

    // ======================================================================================= //

        void    SetupDescriptorPool         (std::vector<Frame> const& p_frames) noexcept;

        void    SetupDescriptorSetLayout    (std::vector<Frame> const& p_frames) noexcept;

        void    SetupDescriptorSets         (std::vector<Frame> const& p_frames) noexcept;

        void    SetupPipelineLayout         (std::vector<Frame> const& p_frames) noexcept;

        void    Setup2DShadowPipeline       (std::vector<Frame> const& p_frames) noexcept;

        void    SetupCubeShadowPipeline     (std::vector<Frame> const& p_frames) noexcept;

};  // !class ShadowPass

#endif // !__VULKAN_SHADOW_PASS_HPP__