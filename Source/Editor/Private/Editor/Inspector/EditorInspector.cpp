#include "PCH.hpp"

#include "Inspector/EditorInspector.hpp"
#include "Components/PrimitiveComponent.hpp"
#include "Components/BoxComponent.hpp"
#include "Components/SphereComponent.hpp"
#include "Components/CapsuleComponent.hpp"
#include "Light/DirectionalLightComponent.hpp"
#include "Light/PointLightComponent.hpp"
#include "Light/SpotLightComponent.hpp"
#include "Camera/CameraComponent.hpp"

// ============================== [Public Constructor and Destructor] ============================== //

EditorInspector::EditorInspector    ()
{}

EditorInspector::~EditorInspector   ()
{}

// ============================== [Public Local Methods] ============================== //

void    EditorInspector::Update ()                              noexcept
{
    ImGui::Begin("Inspector");

    Entity* entity = m_selectedEntityDelegate.Execute();

    if (entity != nullptr && !entity->IsPendingKill())
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);

        ImGui::PushID(entity);

        ShowEntity(entity);

        ImGui::Separator();

        ShowSceneComponents(entity);

        ShowEntityComponents(entity);

        if (m_selectedObject != nullptr)
        {
            ImGui::Text("Properties:");

            DisplayData(m_selectedObject);
        }

        ImGui::PopID();

        ImGui::PopStyleVar();
    }

    ImGui::End();
}

// ============================== [Private Local Methods] ============================== //

void    EditorInspector::ShowSceneComponent     (SceneComponent*    p_component)    noexcept
{
    if (p_component == nullptr)
        return;

    ImGui::AlignTextToFramePadding();

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        
    if (p_component->GetChildrenCount() == 0)
        flags |= ImGuiTreeNodeFlags_Leaf;

    if (m_selectedObject == p_component)
        flags |= ImGuiTreeNodeFlags_Selected;

    bool isOpen = ImGui::TreeNodeEx(std::string(p_component->GetName()).size() != 0 ? p_component->GetName() : "SceneComponent", flags);

    if (ImGui::IsItemClicked())
        m_selectedObject = p_component;

    if (isOpen)
    {
        for (auto children : p_component->GetAttachChildrens())
            ShowSceneComponent(children);

        ImGui::TreePop();
    }
}

void    EditorInspector::ShowSceneComponents    (Entity*            p_entity)   noexcept
{
    SceneComponent* root = p_entity->GetRootComponent();

    if (root != nullptr)
    {
        ImGui::Text("Components:");

        ShowSceneComponent(root);
    }
}

void    EditorInspector::ShowEntityComponent    (EntityComponent*   p_component)    noexcept
{
    if (p_component == nullptr)
        return;

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;

    if (m_selectedObject == p_component)
        flags |= ImGuiTreeNodeFlags_Selected;

    bool isOpen = ImGui::TreeNodeEx(std::string(p_component->GetName()).size() != 0 ? p_component->GetName() : "EntityComponent", flags);

    if (ImGui::IsItemClicked())
        m_selectedObject = p_component;

    if (isOpen)
        ImGui::TreePop();
}

void    EditorInspector::ShowEntityComponents   (Entity*            p_entity)   noexcept
{
    std::vector<EntityComponent*> entityComponents = p_entity->GetComponents();

    if (entityComponents.size() != 0)
    {
        for (auto component : entityComponents)
        {
            if (dynamic_cast<SceneComponent*>(component) == nullptr)
            {
                ShowEntityComponent(component);
            }
        }

        ImGui::Separator();
    }
}

void    EditorInspector::ShowEntity             (Entity*            p_entity)   noexcept
{
    if (m_selectedEntity != p_entity)
    {
        m_selectedEntity = p_entity;
        m_selectedObject = nullptr;
    }

    if (p_entity->IsPendingKill())
        return;

    ImGui::AlignTextToFramePadding();

    ImGui::Text("Entity:");

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;

    if (m_selectedObject == p_entity)
        flags |= ImGuiTreeNodeFlags_Selected;

    if (ImGui::TreeNodeEx(std::string(p_entity->GetName()).size() != 0 ? p_entity->GetName() : "Entity", flags))
    {
        if (ImGui::IsItemClicked())
            m_selectedObject = p_entity;

        ImGui::TreePop();
    }
}

template <typename T>
void    EditorInspector::SetInputField          (T*                 p_object)
{
    Reflect::DoForAllMembers<T>([this, p_object](const auto& p_member)
    {
        std::string name = p_member.GetName();

        name = name.substr(2);

        name[0] = toupper(name[0]);

        if (name == "InstanceID" || name == "InstanceType" || name == "IsPendingKill")
            return;

        if (ImGui::TreeNode(name.c_str()))
        {
            ImGui::BeginGroup();

            ImGui::PushID(&p_member);

            using MemberT = Reflect::GetMemberType<decltype(p_member)>;

            auto value = p_member.Get(*p_object);

            SetInputField(&value);

            Reflect::SetMemberValue<MemberT>(*p_object, p_member.GetName(), value);

            ImGui::PopID();

            ImGui::EndGroup();
            ImGui::TreePop();

            if (std::is_same_v<MemberT, Transform>)
            {
                reinterpret_cast<SceneComponent*>(p_object)->SetWorldTransform(*reinterpret_cast<Transform*>(&value));
                reinterpret_cast<SceneComponent*>(p_object)->UpdateLocalToWorld();
            }

			if (std::is_same_v<MemberT, std::shared_ptr<Model>>)
				reinterpret_cast<StaticMeshComponent*>(p_object)->SetModel(*reinterpret_cast<std::shared_ptr<Model>*>(&value));

            if (std::is_base_of_v<SceneComponent, T> && name == "IsVisibleInGame")
                reinterpret_cast<SceneComponent*>(p_object)->SetVisibilityInGame(reinterpret_cast<SceneComponent*>(p_object)->IsVisible(), true);
        }

        ImGui::Separator();
    });
}

template <typename T>
void    EditorInspector::SetInputField          (std::vector<T>*    p_vector)
{
    size_t size = p_vector->size();

    ImGui::Text("Size");
    ImGui::DragScalar("", ImGuiDataType_U64, &size, 1.0f);

    if (ImGui::TreeNode("Values"))
    {
        for (int i = 0; i < p_vector->size(); i++)
        {
            T& value = (*p_vector)[i];
            int id = static_cast<int>(typeid(T).hash_code()) + i;

            ImGui::PushID(id);

            ImGui::AlignTextToFramePadding();
            ImGui::Text((std::to_string(i) + ": ").c_str());
            ImGui::SameLine();

            SetInputField(&value);

            ImGui::PopID();
        }

        ImGui::TreePop();
    }

    p_vector->resize(size);
}

void    EditorInspector::SetInputField          (Vector3*           p_vector)
{
    ImGui::PushItemWidth(70);

    ImGui::PushID(&(p_vector->m_x));

    ImGui::DragFloat("", &(p_vector->m_x), 1.0f, 0.0f, 0.0f, "X: %.2f", 0.1f);
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::PushID(&(p_vector->m_y));
    ImGui::DragFloat("", &(p_vector->m_y), 1.0f, 0.0f, 0.0f, "Y: %.2f", 0.1f);
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::PushID(&(p_vector->m_z));
    ImGui::DragFloat("", &(p_vector->m_z), 1.0f, 0.0f, 0.0f, "Z: %.2f", 0.1f);

    ImGui::PopID();

    ImGui::PopItemWidth();
}

void    EditorInspector::SetInputField          (Transform*         p_value)
{
    ImGui::PushID(&p_value->m_translation);
    ImGui::Text("Position");
    SetInputField(&p_value->m_translation);
    ImGui::PopID();

    ImGui::PushID(&p_value->m_rotation);
    ImGui::Text("Rotation");
    SetInputField(&p_value->m_rotation);
    ImGui::PopID();

    ImGui::PushID(&p_value->m_scale);
    ImGui::Text("Scale");
    SetInputField(&p_value->m_scale);
    ImGui::PopID();
}

void    EditorInspector::SetInputField          (Color*             p_value)
{
    ImGui::ColorEdit4("", &p_value->m_r, ImGuiColorEditFlags_NoLabel);
}