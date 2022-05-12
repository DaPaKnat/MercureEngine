#ifndef __VULKAN_TONEMAPPING_PASS_HPP__
#define __VULKAN_TONEMAPPING_PASS_HPP__

#include "RenderPass.hpp"

class ENGINE_API TonemappingPass : public RenderPass
{
    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        TonemappingPass () = delete;

        TonemappingPass (std::vector<Frame> const& p_frames);

        ~TonemappingPass();

    // ============================== [Public Local Methods] ============================== //

        void    Rebuild (std::vector<Frame> const&  p_frames)   noexcept final override;

        void    Draw    (Frame              const&  p_frame)    noexcept final override;

    protected:

    // ============================== [Protected Local Properties] ============================== //

        std::vector<VkFramebuffer>      m_framebuffers;

        VkDescriptorPool                m_descriptorPool;

        VkDescriptorSetLayout           m_descriptorSetLayout;

        std::vector<VkDescriptorSet>    m_descriptorSets;

        VkPipelineLayout                m_pipelineLayout;

        VkPipeline                      m_pipeline;

    // ============================== [Protected Local Methods] ============================== //

        void    SetupRenderPass     (std::vector<Frame> const& p_frames) noexcept final override;

        void    SetupFramebuffers   (std::vector<Frame> const& p_frames) noexcept final override;

        void    SetupPipelines      (std::vector<Frame> const& p_frames) noexcept final override;

    // ======================================================================================= //

        void    SetupDescriptorPool         (std::vector<Frame> const& p_frames) noexcept;

        void    SetupDescriptorSetLayout    (std::vector<Frame> const& p_frames) noexcept;

        void    SetupDescriptorSets         (std::vector<Frame> const& p_frames) noexcept;

        void    SetupPipelineLayout         (std::vector<Frame> const& p_frames) noexcept;

        void    SetupPipeline               (std::vector<Frame> const& p_frames) noexcept;

};  // !class TonemappingPass

#endif // !__VULKAN_TONEMAPPING_PASS_HPP__