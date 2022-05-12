#include "PCH.hpp"

#include "Hierarchy/EditorHierarchy.hpp"
#include "ObjectFactory.hpp"

// ============================== [Public Constructor and Destructor] ============================== //

EditorHierarchy::EditorHierarchy    ()
{
    m_inputBuffer.push_back(0);
    m_entityTypes = ObjectFactory::GetDerivedEntityNames();
}

EditorHierarchy::~EditorHierarchy   ()
{}

// ============================== [Private Static Methods] ============================== //

int EditorHierarchy::TextInputCallBack  (ImGuiInputTextCallbackData*    p_data)
{
    if (p_data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        ImVector<char>* my_str = (ImVector<char>*)p_data->UserData;

        IM_ASSERT(my_str->begin() == p_data->Buf);

        my_str->resize(p_data->BufSize);

        p_data->Buf = my_str->begin();
    }

    return 0;
}

// ============================== [Private Local Methods] ============================== //

void     EditorHierarchy::EntityPopup        ()                          noexcept
{
    if (ImGui::BeginPopupModal("Create Entity", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::AlignTextToFramePadding();

        ImGui::Text("Name:");

        ImGui::SameLine();

        if (ImGui::InputText("", m_inputBuffer.begin(), (size_t)m_inputBuffer.size(),
            ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackResize,
            EditorHierarchy::TextInputCallBack, (void*)&m_inputBuffer)
            || ImGui::Button("Create"))
        {
            SpawnEntity(m_inputBuffer.Data);

            Level* level = GEngine->GetWorld()->GetCurrentLevel();

            if (level)
                level->GetEntities()[GEngine->GetWorld()->GetEntityCount() - 1]->SetWorldLocation(
                    GEngine->GetWorld()->GetMainCamera()->GetWorldLocation() + GEngine->GetWorld()->GetMainCamera()->GetForward() * 5);

            m_inputBuffer.clear();
            m_inputBuffer.push_back(0);
            m_entityType.clear();
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            m_entityType.clear();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void     EditorHierarchy::ShowEntity         (Entity*        p_entity)   noexcept
{
    if (p_entity != nullptr && !p_entity->IsPendingKill())
    {
        ImGui::PushID(p_entity);
        ImGui::AlignTextToFramePadding();

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;

        if (m_selectedEntity == p_entity)
            flags |= ImGuiTreeNodeFlags_Selected;

        bool node_open = ImGui::TreeNodeEx(std::string(p_entity->GetName()) != "" ? p_entity->GetName() : "Entity", flags);

        if (ImGui::IsItemClicked())
            m_selectedEntity = p_entity;

        if (node_open)
            ImGui::TreePop();

        ImGui::PopID();
    }
}