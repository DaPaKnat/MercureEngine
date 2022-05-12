#ifndef __EDITOR_HPP__
#define __EDITOR_HPP__

#include "RHI.hpp"
#include "Application.hpp"
#include "EditorWindow.hpp"
#include "Camera/EditorCamera.hpp"

#include "GameFramework/Entity.hpp"
#include "Components/StaticMeshComponent.hpp"
#include "Level.hpp"

DECLARE_LOG_CATEGORY(LogEditor);

// ============================== [Data Structure] ============================== //

struct EditorFrame
{
    CommandBuffer   commandBuffer;
    Buffer          vertexBuffer;
    Buffer          indexBuffer;
    VkFence         fence;
    VkSemaphore     imageAvailableSemaphore;
    VkSemaphore     renderFinishedSemaphore;
    VkFramebuffer   framebuffer;

};  // !struct EditorFrame

struct EditorLayout
{
    ImGuiID     dockspaceID;

    ImGuiID     logsDockID;

    ImGuiID     metricsDockID;

    ImVec2      dockspaceSize = ImVec2(0, 0);

    bool        isInitialized = false;
};

// ============================================================================== //

class Editor : public UniqueObject
{
    public:

    // ============================== [Public Constructor and Destructor] ============================== //

        Editor  ();

        ~Editor ();

    // ============================== [Public Local Methods] ============================== //

        void                        Run                         ();

        void                        Close                       ()                          noexcept;

        World*                      GetWorld                    ()                          noexcept;

        void                        TooglePlay                  ()                          noexcept;

        void                        CreateLevel                 (std::string const& p_name) noexcept;

        void                        OpenLevel                   (std::string const& p_name) noexcept;

        void                        LoadLevel                   (std::string const& p_name) noexcept;

        void                        ImportLevel                 (std::string const& p_name) noexcept;

        void                        SaveLevel                   ()                          noexcept;

        Entity* const*              GetLevelEntities            ()                          noexcept;

        size_t                      GetLevelEntityCount         ()                          noexcept;

        Entity*                     GetSelectedEntity           ()                          noexcept;

        void                        SpawnEntity                 (std::string const& p_name) noexcept;

        std::vector<std::string>    GetLevelNames               ()                          noexcept;

        EditorCamera&               GetEditorCamera             ()                          noexcept;
    
        bool                        IsLevelOpen                 (std::string_view   p_name) const noexcept;

        bool                        HasAnyLevelOpen             ()                          const noexcept;

    private:

    // ============================== [Private Static Methods] ============================== //

        static ANSICHAR const*  ImGuiGetClipboardText   (void*              p_userData) noexcept;

        static void             ImGuiSetClipboardText   (void*              p_userData,
                                                         ANSICHAR const*    p_text)     noexcept;

    // ============================== [Private Local Properties] ============================== //

        std::unique_ptr<Engine>     m_engine;

        ImGuiContext*               m_context;

        std::vector<EditorWindow*>  m_windows;

        EditorLayout                m_layoutInfo;

        EditorCamera                m_editorCamera;

        EditorWindow*               m_hierarchy;

    // ======================================================================================== //

        Image                       m_font;

        VkRenderPass                m_renderPass;

        VkDescriptorPool            m_descriptorPool;

        VkDescriptorSetLayout       m_descriptorSetLayout;

        VkDescriptorSet             m_descriptorSet;

        VkPipelineLayout            m_pipelineLayout;

        VkPipeline                  m_pipeline;

        std::vector<EditorFrame>    m_frames;

        Level                       m_levelData;

    // ============================== [Private Local Methods] ============================== //

        void    SetupImGui                  ()  noexcept;

        void    SetupLayout                 ()  noexcept;

        void    SetupDocking                ()  noexcept;

        void    LinkDocking                 ()  noexcept;

        void    SetupFont                   ()  noexcept;

        void    SetupRenderPass             ()  noexcept;

        void    SetupDescriptorPool         ()  noexcept;

        void    SetupDescriptorSetLayout    () noexcept;

        void    SetupDescriptorSets         ()  noexcept;

        void    SetupPipelineLayout         ()  noexcept;

        void    SetupPipeline               ()  noexcept;

        void    SetupFrames                 ()  noexcept;

        void    FocusOnEntity               ()  noexcept;

    // ======================================================================================== //

        void    UpdateWindow                (ImGuiIO&           p_IO,
                                             ImGuiPlatformIO&   p_platformIO)       noexcept;

        void    UpdateMousePosAndButtons    (ImGuiIO&           p_IO,
                                             ImGuiPlatformIO&   p_platformIO)       noexcept;

        void    UpdateMouseCursor           (ImGuiIO&           p_IO,
                                             ImGuiPlatformIO&   p_platformIO)       noexcept;

        void    UpdateLayout                ()                                      noexcept;

    // ======================================================================================== //

        void    Rebuild ()  noexcept;

        void    Draw    ()  noexcept;

    // ======================================================================================== // 

        void    ImGuiMouseButtonCallback    (EButton    p_button,
                                             EState     p_state)    noexcept;

        void    ImGuiScrollCallback         (double     p_xOffset,
                                             double     p_yOffset)  noexcept;

        void    ImGuiKeyCallback            (EKey       p_key,
                                             EState     p_state)    noexcept;

        void    ImGuiCharCallback           (uint32     p_code)     noexcept;

}; // !class Editor

#endif // !__EDITOR_HPP__