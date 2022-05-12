#ifndef __VULKAN_RHI_HPP__
#define __VULKAN_RHI_HPP__

#include "EngineModule.hpp"

#include "Object/Device.hpp"
#include "Object/Instance.hpp"
#include "Object/Swapchain.hpp"
#include "Object/PipelineCache.hpp"
#include "Object/DeviceAllocator.hpp"

#include "Vulkan/RenderPasses/RenderPass.hpp"

// ============================== [Data Structure] ============================== //

struct ENGINE_API Frame
{
    // ============================== [RHI Data] ============================== //

        uint32        index;
        Image         result;
        CommandBuffer commandBuffer;
        VkFence       fence;
        VkSemaphore   imageAvailableSemaphore;
        VkSemaphore   renderFinishedSemaphore;

        struct
        {
            Buffer camera;
            Buffer pointLight;
            Buffer spotLight;
            Buffer directionalLight;

        }   ubos;

        struct
        {
            VkDescriptorSet scene;
            VkDescriptorSet camera;
            VkDescriptorSet light;

        }   descriptorSets;

    // ============================== [Renderer Data] ============================== //

        struct RenderList* renderList;

};  // !struct Frame;

// ============================================================================== //

class ENGINE_API RHI : public EngineModule
{
    public:

        using RenderStageList = std::map<ERenderStage, std::unique_ptr<RenderPass>>;

    // ============================== [Public Static Methods] ============================== //

        static INLINE RHI& Get() noexcept
        {
            return GEngine->GetModuleManager()->Get<RHI>();
        }

    // ============================== [Public Constructor and Destructor] ============================== //

        RHI ();

        ~RHI() = default;

    // ============================== [Module Public Local Methods] ============================== //
        
        /**
         * Initializes the Vulkan API and the RHI.
         */
        virtual void    Initialize  (EngineKey const& p_passkey)   final override;

        /**
         * This function actually does nothing.
         */
        virtual void    Update      (EngineKey const& p_passkey)   final override;

        /**
         * Shutdowns the Vulkan API and the RHI.
         */
        virtual void    Shutdown    (EngineKey const& p_passkey)   final override;

    // ============================== [Public Local Methods] ============================== //

        Frame&  NextFrame   (EngineKey const& p_passkey);

        void    DrawFrame   (EngineKey const& p_passkey);

    // ==================================================================================== //

        INLINE std::unique_ptr<Instance>        const&  GetInstance         ()                      const noexcept  { return m_instance; }

        INLINE std::unique_ptr<Device>          const&  GetDevice           ()                      const noexcept  { return m_device; }

        INLINE std::unique_ptr<Swapchain>       const&  GetSwapchain        ()                      const noexcept  {return m_swapchain; }

        INLINE std::unique_ptr<DeviceAllocator> const&  GetAllocator        ()                      const noexcept  { return m_allocator; }

        INLINE std::unique_ptr<PipelineCache>   const&  GetPipelineCache    ()                      const noexcept  { return m_cache; }

        INLINE Frame                            const&  GetCurrentFrame     ()                      const noexcept  { return m_frames[m_currentFrame]; }

        INLINE std::vector<Frame>               const&  GetFrames           ()                      const noexcept  { return m_frames; }

        INLINE std::unique_ptr<RenderPass>      const&  GetRenderPass       (ERenderStage p_stage)  const noexcept  { return m_renderPasses.at(p_stage); }

        INLINE VkSampler                        const   GetSceneSampler     ()                      const noexcept  { return m_samplers.scene; }

        INLINE VkSampler                        const   GetTextureSampler   ()                      const noexcept  { return m_samplers.texture; }

        INLINE VkDescriptorSetLayout            const   GetCameraLayout     ()                      const noexcept  { return m_descriptorSetLayouts.camera; }

        INLINE VkDescriptorSetLayout            const   GetLightLayout      ()                      const noexcept  { return m_descriptorSetLayouts.light; }

    private:

    // ============================== [Private Local Properties] ============================== //

        uint32                              m_currentFrame;

        std::unique_ptr<Instance>           m_instance;

        std::unique_ptr<Device>             m_device;

        std::unique_ptr<Swapchain>          m_swapchain;

        std::unique_ptr<DeviceAllocator>    m_allocator;

        std::unique_ptr<PipelineCache>      m_cache;

        std::vector<Frame>                  m_frames;

        RenderStageList                     m_renderPasses;

        VkDescriptorPool                    m_descriptorPool;

        struct
        {
            VkSampler scene;
            VkSampler texture;

        }   m_samplers;

        struct
        {
            VkDescriptorSetLayout scene;
            VkDescriptorSetLayout camera;
            VkDescriptorSetLayout light;

        }   m_descriptorSetLayouts;

    // ============================== [Private Local Methods] ============================== //

        void    Cleanup                 ()                  noexcept;

        void    UpdateUniformBuffers    (Frame& p_frame)    noexcept;

    // ===================================================================================== //

        void    SetupSamplers               () noexcept;

        void    SetupFrames                 () noexcept;

        void    SetupUniformBuffers         () noexcept;

        void    SetupDescriptorPool         () noexcept;

        void    SetupDescriptorSetLayouts   () noexcept;

        void    SetupDescriptorSets         () noexcept;

};  // !class RHI

#endif // !__VULKAN_RHI_HPP__