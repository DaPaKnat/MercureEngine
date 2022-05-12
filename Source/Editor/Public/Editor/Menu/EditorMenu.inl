#ifndef __EDITOR_MENU_INL__ 
#define __EDITOR_MENU_INL__

#include "Editor.hpp"

// ============================== [Public Local Methods] ============================== //

INLINE void EditorMenu::Init    (Editor*    p_editor)   noexcept
{
    m_tooglePlayDelegate.Bind<Editor, &Editor::TooglePlay>(p_editor);
    m_createLevelDelegate.Bind<Editor, &Editor::CreateLevel>(p_editor);
    m_LoadLevelDelegate.Bind<Editor, &Editor::OpenLevel>(p_editor);
    m_saveLevelDelegate.Bind<Editor, &Editor::SaveLevel>(p_editor);
    m_closeDelegate.Bind<Editor, &Editor::Close>(p_editor);
    m_levelNamesDelegate.Bind<Editor, &Editor::GetLevelNames>(p_editor);
    m_importLevelDelegate.Bind<Editor, &Editor::ImportLevel>(p_editor);
    m_editorCameraDelegate.Bind<Editor, &Editor::GetEditorCamera>(p_editor);

    m_cameraParameters.m_cameraRotation = m_editorCameraDelegate.Execute().GetRotationSpeed();
    m_cameraParameters.m_cameraMovement = m_editorCameraDelegate.Execute().GetMovementSpeed();
    m_cameraParameters.m_focusDistance  = m_editorCameraDelegate.Execute().GetFocusDistance();
    m_cameraParameters.m_focusSpeed     = m_editorCameraDelegate.Execute().GetFocusSpeed();
}

INLINE void EditorMenu::Update  ()                      noexcept
{
    if (ImGui::BeginMenuBar())
    {
        FileMenu();

        EditMenu();

        PlayButton();
    }

    CheckPopups();

    ImGui::EndMenuBar();
}

// ============================== [Private Local Methods] ============================== //

INLINE void EditorMenu::FileMenu        ()  noexcept
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New level", "CTRL+N"))
            m_newLevel = true;

        if (ImGui::MenuItem("Open level", "CTRL+O"))
            m_loadLevel = true;

        if (ImGui::BeginMenu("Import level", "CTRL+SHIFT+O"))
            ImportLevel();

        ImGui::Separator();

        if (ImGui::MenuItem("Save level", "CTRL+S"))
            m_saveLevelDelegate.ExecuteIfBound();

        if (ImGui::MenuItem("Save all", "CTRL+SHIFT+S"))
            GEngine->GetWorld()->SaveAll();

        ImGui::Separator();

        if (ImGui::MenuItem("Exit"))
            m_closeDelegate.ExecuteIfBound();

        ImGui::EndMenu();
    }
}

INLINE void EditorMenu::EditMenu        ()  noexcept
{
    if (ImGui::BeginMenu("Edit"))
    {
        ImGui::MenuItem("Undo", "CTRL+Z", (bool*)0, m_canUndo);

        ImGui::MenuItem("Redo", "CTRL+Y", (bool*)0, m_canRedo);

        ImGui::Separator();

        ImGui::MenuItem("Copy", "CTRL+C", (bool*)0, m_canCopy);
        ImGui::MenuItem("Paste", "CTRL+V", (bool*)0, m_canPaste);
        ImGui::MenuItem("Cut", "CTRL+X", (bool*)0, m_canCut);

        ImGui::Separator();

        ImGui::MenuItem("Duplicate", "CTRL+D", (bool*)0, m_canDuplicate);
        ImGui::MenuItem("Delete", "Del", (bool*)0, m_canDelete);

        ImGui::Separator();

        if (ImGui::BeginMenu("Preference"))
        {
            if (ImGui::BeginMenu("Theme"))
            {
                if (ImGui::MenuItem("Classic"))
                    ImGui::StyleColorsClassic();

                if (ImGui::MenuItem("Dark"))
                    ImGui::StyleColorsDark();

                if (ImGui::MenuItem("Light"))
                    ImGui::StyleColorsLight();

                if (ImGui::MenuItem("Mercure"))
                    StyleMercure();

                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Camera"))
                m_setCameraParameters = true;

            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }
}

INLINE void EditorMenu::PlayButton      ()  noexcept
{
    ImVec2 size = ImGui::GetItemRectSize();

    size.y += 2;

    ImGui::SetCursorPos(ImVec2(ImGui::GetCurrentWindow()->Size.x / 2 - size.x / 2, 0));

    if (ImGui::Button(GEngine->GetWorld()->HasBegunPlay() ? "End" : "Play", size))
        m_tooglePlayDelegate.ExecuteIfBound();
}

#endif // !__EDITOR_MENU_INL__