#ifndef __EDITOR_HIERARCHY_INL__
#define __EDITOR_HIERARCHY_INL__

#include "Editor.hpp"

// ============================== [Public Local Methods] ============================== //

INLINE void     EditorHierarchy::Init               (Editor*    p_editor)       noexcept
{
    m_levelEntitiesDelegate.Bind<Editor, &Editor::GetLevelEntities>(p_editor);
    m_levelEntityCountDelegate.Bind<Editor, &Editor::GetLevelEntityCount>(p_editor);
}

INLINE void     EditorHierarchy::Update             ()                          noexcept
{
    ImGui::Begin("Hierarchy", NULL, ImGuiWindowFlags_MenuBar);

    MenuBar();

    std::string title = "Current level: " + std::string(GEngine && GEngine->GetWorld() && GEngine->GetWorld()->GetCurrentLevel() ? GEngine->GetWorld()->GetCurrentLevel()->GetName() : "None");

    ImGui::Text(title.c_str());

    ImGui::Separator();

    Entity* const* entities = m_levelEntitiesDelegate.Execute();

    for (int i = 0; i < m_levelEntityCountDelegate.Execute(); i++)
        ShowEntity(entities[i]);

    if (m_selectedEntity)
    {
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete)))
        {
            GEngine->GetWorld()->DestroyEntity(m_selectedEntity);
            m_selectedEntity = nullptr;
        }
    }

    ImGui::End();
}

INLINE void     EditorHierarchy::MenuBar            ()                          noexcept
{
    ImGui::BeginMenuBar();

    if (ImGui::BeginMenu("Create"))
    {
        for (auto const& type : m_entityTypes)
        {
            if (ImGui::MenuItem(type.c_str()))
                m_entityType = type;
        }
        
        ImGui::EndMenu();
    }

    ImGui::Separator();

    ImGui::EndMenuBar();

    CheckPopup();
}

INLINE void     EditorHierarchy::CheckPopup         ()                          noexcept
{
    if (m_entityType != "")
        ImGui::OpenPopup("Create Entity");

    EntityPopup();
}

INLINE Entity*  EditorHierarchy::GetSelectedEntity  ()                          noexcept
{
    return m_selectedEntity;
}

#endif // !__EDITOR_HIERARCHY_INL__