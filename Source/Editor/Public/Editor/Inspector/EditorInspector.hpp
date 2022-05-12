#ifndef __EDITOR_INSPECTOR_HPP__
#define __EDITOR_INSPECTOR_HPP__

#include "EditorWindow.hpp"
#include "Asset.hpp"
#include "Vulkan/Asset/Model/Model.hpp"
#include "Vulkan/Asset/Material/Material.hpp"
#include "Vulkan/Asset/Texture/Texture.hpp"
#include "GameFramework/Entity.hpp"

DECLARE_DELEGATE_RetVal(Entity*, EditorSelectedEntity);

class EditorInspector : public EditorWindow
{
    public:

    // ============================== [Public Constructor and Destructor] ============================== //

        EditorInspector  ();

        ~EditorInspector ();

     // ============================== [Public Local Methods] ============================== //

        void    Init                (class Editor*      p_editor)       noexcept override;

        void    Update              ()                                  noexcept override;

    private:

    // ============================== [Public Local Properties] ============================== //

        EditorSelectedEntity    m_selectedEntityDelegate;

        Object*                 m_selectedObject = nullptr;

        Entity*                 m_selectedEntity = nullptr;
        
    // ============================== [Public Local Methods] ============================== //
        
        void    ShowSceneComponent      (SceneComponent*                    p_component)    noexcept;

        void    ShowSceneComponents     (Entity*                            p_entity)       noexcept;

        void    ShowEntityComponent     (EntityComponent*                   p_component)    noexcept;

        void    ShowEntityComponents    (Entity*                            p_entity)       noexcept;

        void    ShowEntity              (Entity*                            p_entity)       noexcept;

        void    DisplayData             (Object*                            p_object)       noexcept;

        template <typename T>
        void    SetInputField           (T*                                 p_object);

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<Object, T>>>
        void    SetInputField           (T**                                p_value)
        {
            ImGui::Button(*p_value == nullptr ? "nullptr" : dynamic_cast<Object*>(*p_value)->GetName());

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(typeid(T).name()))
                {
                    IM_ASSERT(payload->DataSize == sizeof(T*));

                    T* data = *(T**)payload->Data;

                    *p_value = data;
                }

                ImGui::EndDragDropTarget();
            }
        }

        void    SetInputField           (std::string*                       p_member);

        void    SetInputField           (bool*                              p_value);
        
        void    SetInputField           (float*                             p_value);

        void    SetInputField           (int*                               p_value);

        void    SetInputField           (size_t*                            p_value);

        template <typename T>
        void    SetInputField           (std::vector<T*>*                   p_vector);

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<Asset, T>>>
        void    SetInputField           (std::shared_ptr<T>*                p_value)
        {
            ImGui::Button(p_value->get() == nullptr ? "nullptr" : dynamic_cast<Asset*>(p_value->get())->GetName().c_str());

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(typeid(T).name()))
                {
                    IM_ASSERT(payload->DataSize == sizeof(std::shared_ptr<T>));

                    std::shared_ptr<T> data = *(std::shared_ptr<T>*)payload->Data;

                    *p_value = data;
                }

                ImGui::EndDragDropTarget();
            }
        }

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<Asset, T>>>
        void    SetInputField           (std::vector<std::shared_ptr<T>>*   p_vector)
        {
            if (ImGui::TreeNode("Values"))
            {
                for (int i = 0; i < p_vector->size(); i++)
                {
                    std::shared_ptr<T>& value = (*p_vector)[i];

                    ImGui::PushID(static_cast<int>(typeid(T).hash_code()) + i);

                    ImGui::AlignTextToFramePadding();
                    ImGui::Text((std::to_string(i) + ": ").c_str());
                    ImGui::SameLine();

                    SetInputField(&value);

                    ImGui::PopID();
                }

                ImGui::TreePop();
            }
        }

        template <typename T>
        void    SetInputField           (std::vector<T>*                    p_vector);

        void    SetInputField           (Quaternion*                        p_value);

        void    SetInputField           (Vector3*                           p_vector);

        void    SetInputField           (Transform*                         p_value);

        void    SetInputField           (Color*                             p_value);

}; // !class EditorInspector

#include "EditorInspector.inl"
#include "EditorInspector.generated.hpp"

#endif // !__EDITOR_INSPECTOR_HPP__