#include "PCH.hpp"

#include "Menu/EditorMenu.hpp"

// ============================== [Public Constructor and Destructor] ============================== //

EditorMenu::EditorMenu  ()
{
    m_inputBuffer.push_back(0);

    StyleMercure();
}

EditorMenu::~EditorMenu ()
{}

// ============================== [Private Static Methods] ============================== //

int EditorMenu::TextInputCallBack   (ImGuiInputTextCallbackData*    p_data)
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

void    EditorMenu::NewLevel            ()  noexcept
{
    if (ImGui::BeginPopupModal("New level", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::AlignTextToFramePadding();

        ImGui::Text("Name:");

        ImGui::SameLine();

        if (ImGui::InputText("", m_inputBuffer.begin(), (size_t)m_inputBuffer.size(), 
            ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackResize, 
            EditorMenu::TextInputCallBack, (void*)&m_inputBuffer) || 
            ImGui::Button("Create"))
        {
            m_createLevelDelegate.ExecuteIfBound(m_inputBuffer.Data);

            m_newLevel = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel") || ImGui::IsKeyPressed(static_cast<int32>(EKey::ESCAPE)))
        {
            m_newLevel = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void    EditorMenu::LoadLevel           ()  noexcept
{
    if (ImGui::BeginPopupModal("Open level", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::AlignTextToFramePadding();

        ImGui::Text("Level list");

        {
            ImGui::BeginChild("Current levels", ImVec2(400, 50), true, ImGuiWindowFlags_NoCollapse);

            std::vector<std::string> levelNames { m_levelNamesDelegate.Execute() };

            for (auto level : levelNames)
            {
                if (ImGui::Selectable(level.c_str()))
                {
                    m_LoadLevelDelegate.ExecuteIfBound(level);

                    m_loadLevel = false;
                    ImGui::CloseCurrentPopup();

                    break;
                }
            }

            ImGui::EndChild();
        }

        if (ImGui::Button("Cancel") || ImGui::IsKeyPressed(static_cast<int32>(EKey::ESCAPE)))
        {
            m_loadLevel = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void    EditorMenu::ImportLevel         ()  noexcept
{
    std::string name;

    for (auto const& file : std::filesystem::directory_iterator(ASSET_DIRECTORY + std::string("/Levels/")))
    {
        name = file.path().filename().string();
        
        name = name.substr(0, name.find('.'));

        ImGui::PushID(static_cast<int>(std::hash<std::string>()(name)));

        if (ImGui::MenuItem(name.c_str()))
            m_importLevelDelegate.ExecuteIfBound(name);

        ImGui::PopID();
    }

    ImGui::EndMenu();
}

void    EditorMenu::SetCameraParameters ()  noexcept
{
    if (ImGui::BeginPopupModal("Editor camera parameters", &m_setCameraParameters, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Camera movement speed: ");
        ImGui::PushID(&m_cameraParameters.m_cameraMovement);
        ImGui::SameLine();
        ImGui::SliderFloat("", &m_cameraParameters.m_cameraMovement, 0.1f, 20.0f, "%.1f");
        ImGui::PopID();

        ImGui::Text("Camera rotation speed: ");
        ImGui::PushID(&m_cameraParameters.m_cameraRotation);
        ImGui::SameLine();
        ImGui::SliderFloat("", &m_cameraParameters.m_cameraRotation, 0.1f, 10.0f, "%.1f");
        ImGui::PopID();

        ImGui::Text("Camera focus speed:    ");
        ImGui::PushID(&m_cameraParameters.m_focusSpeed);
        ImGui::SameLine();
        ImGui::SliderFloat("", &m_cameraParameters.m_focusSpeed, 0.1f, 10.0f, "%.1f");
        ImGui::PopID();

        ImGui::Text("Camera focus distance: ");
        ImGui::PushID(&m_cameraParameters.m_focusDistance);
        ImGui::SameLine();
        ImGui::SliderFloat("", &m_cameraParameters.m_focusDistance, 0.1f, 10.0f, "%.1f");
        ImGui::PopID();

        if (ImGui::Button("Save"))
        {
            m_editorCameraDelegate.Execute().SetRotationSpeed(m_cameraParameters.m_cameraRotation);
            m_editorCameraDelegate.Execute().SetMovementSpeed(m_cameraParameters.m_cameraMovement);
            m_editorCameraDelegate.Execute().SetFocusSpeed(m_cameraParameters.m_focusSpeed);
            m_editorCameraDelegate.Execute().SetFocusDistance(m_cameraParameters.m_focusDistance);

            m_setCameraParameters = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            m_setCameraParameters = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void    EditorMenu::CheckPopups         ()  noexcept
{
    if (m_newLevel)
    {
        ImGui::OpenPopup("New level");
        NewLevel();
    }

    if (m_loadLevel)
    {
        ImGui::OpenPopup("Open level");
        LoadLevel();
    }

    if (m_setCameraParameters)
    {
        ImGui::OpenPopup("Editor camera parameters");
        SetCameraParameters();
    }
}

void    EditorMenu::StyleMercure        ()  noexcept
{
    ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = ImVec4(0.980f, 0.060f, 0.060f, 1.000f);

    ImGui::GetStyle().Colors[ImGuiCol_TabActive] = ImGui::GetStyle().Colors[ImGuiCol_TabUnfocusedActive] =
        ImGui::GetStyle().Colors[ImGuiCol_TabHovered] = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = 
            ImGui::GetStyle().Colors[ImGuiCol_FrameBgActive] = ImGui::GetStyle().Colors[ImGuiCol_HeaderActive] = 
                ImVec4(0.801f, 0.183f, 0.115f, 1.f);

    ImGui::GetStyle().Colors[ImGuiCol_TabUnfocused] = ImGui::GetStyle().Colors[ImGuiCol_Tab] =
        ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0.313f, 0.313f, 0.3136f, 0.9f);

    ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.145f, 0.145f, 0.145f, 1.f);

    ImGui::GetStyle().Colors[ImGuiCol_Separator] = ImGui::GetStyle().Colors[ImGuiCol_TitleBg] =
        ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = ImGui::GetStyle().Colors[ImGuiCol_SeparatorHovered] =
        ImGui::GetStyle().Colors[ImGuiCol_SeparatorActive] = ImVec4(0.210f, 0.210f, 0.210f, 1.f);

    ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg] = ImVec4(0.127f, 0.119f, 0.119f, 1.000f);

    ImGui::GetStyle().Colors[ImGuiCol_CheckMark] = ImVec4(0.089f, 0.873f, 0.053f, 1.000f);

    ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = ImVec4(0.538f, 0.550f, 0.569f, 0.540f);

    ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered] = ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered] =
        ImVec4(0.801f, 0.183f, 0.115f, .8f);

    ImGui::GetStyle().Colors[ImGuiCol_Header] = ImVec4(0.801f, 0.183f, 0.115f, .5f);
}