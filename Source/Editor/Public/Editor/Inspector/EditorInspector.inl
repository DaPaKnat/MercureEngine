#ifndef __EDITOR_INSPECTOR_INL__
#define __EDITOR_INSPECTOR_INL__

#include "Editor.hpp"

// ============================== [Public Local Methods] ============================== //

INLINE void EditorInspector::Init                   (Editor*            p_editor)   noexcept
{
    m_selectedEntityDelegate.Bind<Editor, &Editor::GetSelectedEntity>(p_editor);
} 
             
INLINE void EditorInspector::SetInputField          (std::string*       p_member)
{
    std::string string;

    string.resize(128);

    if (ImGui::InputText("", string.data(), string.size(), ImGuiInputTextFlags_EnterReturnsTrue))
        *p_member = string;
}
                                                    
INLINE void EditorInspector::SetInputField          (bool*              p_value)
{
    ImGui::Checkbox("", p_value);
}
                                                    
INLINE void EditorInspector::SetInputField          (float*             p_value)
{
    ImGui::DragFloat("", p_value);
}
                                                    
INLINE void EditorInspector::SetInputField          (int*               p_value)
{
    ImGui::DragInt("", p_value);
}
                                                    
INLINE void EditorInspector::SetInputField          (size_t*            p_value)
{
    ImGui::DragScalar("", ImGuiDataType_U64, p_value, 1.0f);
}
                                                    
template <typename T>                               
INLINE void EditorInspector::SetInputField          (std::vector<T*>*   p_vector)
{
    for (auto value : *p_vector)
        SetInputField(value);
}

INLINE void EditorInspector::SetInputField          (Quaternion*        p_value)
{
    Vector3 deg = p_value->GetEulerDeg();

    SetInputField(&deg);

    *p_value = p_value->MakeFromEulerDeg(deg);
}

#endif // !__EDITOR_INSPECTOR_INL__