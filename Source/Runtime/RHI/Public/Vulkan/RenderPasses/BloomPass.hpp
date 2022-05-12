#ifndef __VULKAN_BLOOM_PASS_HPP__
#define __VULKAN_BLOOM_PASS_HPP__

#include "RenderPass.hpp"

class ENGINE_API BloomPass : public RenderPass
{
    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        BloomPass   () = delete;

        BloomPass   (std::vector<Frame> const& p_frames);

        ~BloomPass  ();

    // ============================== [Public Local Methods] ============================== //

        void    Rebuild (std::vector<Frame> const&  p_frames)   noexcept final override;

        void    Draw    (Frame              const&  p_frame)    noexcept final override;

    // ==================================================================================== //

        INLINE Image const& GetResult(size_t p_index) const noexcept { return *m_attachments[p_index].horizontalBlur; }

    protected:

    // ============================== [Data Structure] ============================== //

        struct Attachment
        {
            Image const*    horizontalBlur;
            Image const*    verticalBlur;
            VkFramebuffer   brightColorFramebuffer;
            VkFramebuffer   horizontalBlurFramebuffer;
            VkFramebuffer   verticalBlurFramebuffer;
            VkDescriptorSet brightColorDescriptorSet;
            VkDescriptorSet horizontalBlurDescriptorSet;
            VkDescriptorSet verticalBlurDescriptorSet;
        };

    // ============================== [Protected Local Properties] ============================== //

        struct
        {
            VkDescriptorPool      descriptorPool;
            VkDescriptorSetLayout descriptorSetLayout;
            VkPipelineLayout      pipelineLayout;
            VkPipeline            pipeline;

        }   m_brightcolor;

        struct
        {
            VkDescriptorPool      descriptorPool;
            VkDescriptorSetLayout descriptorSetLayout;
            VkPipelineLayout      pipelineLayout;
            VkPipeline            horizontalBlurPipeline;
            VkPipeline            verticalBlurPipeline;

        }   m_blur;

        std::vector<Attachment> m_attachments;

    // ============================== [Protected Local Methods] ============================== //

        void    SetupRenderPass     (std::vector<Frame> const& p_frames) noexcept final override;

        void    SetupFramebuffers   (std::vector<Frame> const& p_frames) noexcept final override;

        void    SetupPipelines      (std::vector<Frame> const& p_frames) noexcept final override;

    // ======================================================================================= //

        void    SetupBrightColorDescriptorPool      (std::vector<Frame> const& p_frames) noexcept;

        void    SetupBrightColorDescriptorSetLayout (std::vector<Frame> const& p_frames) noexcept;

        void    SetupBrightColorDescriptorSets      (std::vector<Frame> const& p_frames) noexcept;

        void    SetupBrightColorPipelineLayout      (std::vector<Frame> const& p_frames) noexcept;

        void    SetupBrightColorPipeline            (std::vector<Frame> const& p_frames) noexcept;

        void    SetupBlurDescriptorPool             (std::vector<Frame> const& p_frames) noexcept;

        void    SetupBlurDescriptorSetLayout        (std::vector<Frame> const& p_frames) noexcept;

        void    SetupBlurDescriptorSets             (std::vector<Frame> const& p_frames) noexcept;

        void    SetupBlurPipelineLayout             (std::vector<Frame> const& p_frames) noexcept;

        void    SetupBlurPipelines                  (std::vector<Frame> const& p_frames) noexcept;

    // ======================================================================================= //
        
        void    BrightColorPass (Frame const& p_frame) const noexcept;

        void    BlurPass        (Frame const& p_frame) const noexcept;

};  // !class BloomPass

#endif // !__VULKAN_BLOOM_PASS_HPP__