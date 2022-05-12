#ifndef __EDITOR_EXPLORER_INL__
#define __EDITOR_EXPLORER_INL__

#include "Editor.hpp"

// ============================== [Public Local Methods] ============================== //

INLINE void EditorExplorer::Init    (Editor*    p_editor)   noexcept
{
    (void)p_editor;

    Application::Get().GetMainWindow()->BindFileDropCallback<EditorExplorer, &EditorExplorer::OnFileDrop>(this);

    m_root = LoadFolder(ASSET_DIRECTORY);

    m_root.name = "Content";
}

INLINE void EditorExplorer::Update  ()                      noexcept
{
    ImGui::Begin("Explorer", NULL, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

    if (nullptr != m_selectedAsset)
        ImGui::Columns(2);

    ShowFolderContent(m_root);

    if (nullptr != m_selectedAsset)
        ImGui::NextColumn();

    ShowAssetInspector();

    ImGui::Columns(1);

    ImGui::End();
}

// ============================== [Private Local Methods] ============================== //

INLINE void EditorExplorer::EditAssetMember (float* p_data)
{
    ImGui::DragFloat("", p_data);
}

#endif // !__EDITOR_EXPLORER_INL__
