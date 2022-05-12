#ifndef __VULKAN_SWAPCHAIN_HPP__
#define __VULKAN_SWAPCHAIN_HPP__

#include "Vulkan/Vulkan.hpp"

// ============================== [Forward Declaration] ============================== //

class Window;

// ============================== [Data Structures] ============================== //

struct SwapchainSupportDetails
{
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   presentModes;

};  // !struct SwapchainSupportDetails

// =============================================================================== //

class ENGINE_API Swapchain : public UniqueObject
{
    public:

    // ============================== [Public Constructor and Destructor] ============================== //

        Swapchain   ();

        ~Swapchain  ();

    // ============================== [Public Local Methods] ============================== //

        /**
         * @thread_safety This function must only be called from the owner thread.
         */
        VkResult AcquireNextImage(VkSemaphore const p_semaphore) noexcept;

    // ==================================================================================== //

        /**
         * @thread_safety This function must only be called from the owner thread.
         */
        INLINE uint32                           GetImageIndex   ()  const noexcept  { return m_imageIndex; }

        /**
         * @thread_safety This function must only be called from the owner thread.
         */
        INLINE VkSwapchainKHR const             GetHandle       ()  const noexcept  { return m_swapchain; }

        /**
         * @thread_safety This function must only be called from the owner thread.
         */
        INLINE VkFormat                         GetFormat       ()  const noexcept  { return m_surfaceFormat.format; }

        /**
         * @thread_safety This function must only be called from the owner thread.
         */
        INLINE VkExtent2D const&                GetExtent       ()  const noexcept  { return m_extent; }

        /**
         * @thread_safety This function must only be called from the owner thread.
         */
        INLINE uint32                           GetImageCount   ()  const noexcept  { return m_imageCount; }

        /**
         * @thread_safety This function must only be called from the owner thread.
         */
        INLINE std::vector<VkImageView> const&  GetImageViews   ()  const noexcept  { return m_imageViews; }

    private:

    // ======================================================================================== //

        std::atomic_bool            m_isOutOfDate;

        uint32                      m_imageIndex;

        VkSurfaceKHR                m_surface;

        VkSwapchainKHR              m_swapchain;

        VkSurfaceFormatKHR          m_surfaceFormat;

        VkExtent2D                  m_extent;

        VkSurfaceTransformFlagsKHR  m_preTransform;

        VkCompositeAlphaFlagBitsKHR m_compositeAlpha;

        VkPresentModeKHR            m_presentMode;

        uint32                      m_imageCount;

        std::vector<VkImage>        m_images;

        std::vector<VkImageView>    m_imageViews;

    // ============================== [Private Local Methods] ============================== //

        void                    CreateSurface           ()                                                      noexcept;

        void                    CreateSwapchain         ()                                                      noexcept;

        void                    ResizeSwapchain         (int32                                  p_width,
                                                         int32                                  p_height)       noexcept;

        void                    RecreateSwapchain       ()                                                      noexcept;

        SwapchainSupportDetails QuerySwapchainDetails   ()                                                      const noexcept;

        void                    ChooseSurfaceFormat     (std::vector<VkSurfaceFormatKHR> const& p_formats)      noexcept;

        void                    ChooseImageExtent       (VkSurfaceCapabilitiesKHR        const& p_capabilities) noexcept;

        void                    ChoosePreTransform      (VkSurfaceCapabilitiesKHR        const& p_capabilities) noexcept;

        void                    ChooseCompositeAlpha    (VkSurfaceCapabilitiesKHR        const& p_capabilities) noexcept;

        void                    ChoosePresentMode       (std::vector<VkPresentModeKHR>   const& p_presentModes) noexcept;

        void                    ChooseImageCount        (VkSurfaceCapabilitiesKHR        const& p_capabilities) noexcept;

};  // !class Swapchain

#endif // !__VULKAN_SWAPCHAIN_HPP__