#include "PCH.hpp"

#include "ObjectManager.hpp"
#include "Application.hpp"
#include "Camera/EditorCamera.hpp"

// ============================== [Public Constructor and Destructor] ============================== //

EditorCamera::EditorCamera  ()  :
    m_editorCameraEntity    { ObjectManager::Get().CreateObject<Entity>("EditorCamera") },
    m_editorCameraComponent { nullptr },
    m_levelCamera           { nullptr },
    m_dragDelta             { ImVec2() },
    m_targetPosition        { Vector3::Zero },
    m_targetRotation        { Vector3::Zero },
    m_rotationSpeed         { 5.0f },
    m_moveSpeed             { 15.0f },
    m_world                 { GEngine->GetWorld() },
    m_isFocusing            { false },
    m_focusStart            { Vector3::Zero },
    m_focusEnd              { Vector3::Zero },
    m_focusTime             { 0.0f },
    m_focusDistance         { 6.0f },
    m_focusSpeed            { 5.0f }
{
    assert(m_world != nullptr);

    m_editorCameraComponent = m_editorCameraEntity->AddComponent<CameraComponent>("CameraComponent");

    m_editorCameraEntity->SetRootComponent(m_editorCameraComponent);

    m_editorCameraComponent->m_fieldOfView    = 45.0f;
    m_editorCameraComponent->m_aspectRatio    = static_cast<float>(Application::Get().GetMainWindow()->GetExtent().width) /
                                                static_cast<float>(Application::Get().GetMainWindow()->GetExtent().height);

    m_editorCameraEntity->SetWorldLocation(Vector3(0.0f, 0.0f, -3.0f));
    m_editorCameraEntity->SetWorldTransform(Matrix4x4::LookAt(m_editorCameraEntity->GetWorldLocation(), Vector3::Zero, Vector3::Up));

    m_editorCameraComponent->m_nearClipPlane = 0.01f;
    m_editorCameraComponent->m_farClipPlane  = 10000.0f;
}

// ============================== [Public Local Methods] ============================== //

void    EditorCamera::Tick      (float const    p_deltaTime)    noexcept
{
    CameraComponent* worldCamera = m_world->GetMainCamera();

    // Check if main camera is a new one (probably a created one).
    if (worldCamera != m_levelCamera && worldCamera != m_editorCameraComponent && !m_world->HasBegunPlay())
    {
        m_levelCamera = worldCamera;
        m_world->SetMainCamera(m_editorCameraComponent);
        return;
    }

    if (m_isFocusing && !m_world->HasBegunPlay())
        ApplyFocus(p_deltaTime);

    if (!ImGui::IsMouseDragging(1) && !ImGui::IsMouseDown(1) || m_world->HasBegunPlay())
        return;

    UpdateDragDelta();

    UpdatePosition(p_deltaTime);

    UpdateRotation(p_deltaTime);

    UpdateCamera();

    ResetTarget();
}

// ============================== [Private Local Methods] ============================== //

void    EditorCamera::UpdatePosition    (float const    p_deltaTime)    noexcept
{
    if (ImGui::IsKeyDown(static_cast<int32>(EKey::W)))
        m_targetPosition += m_editorCameraEntity->GetForward() * p_deltaTime * m_moveSpeed;
    else if (ImGui::IsKeyDown(static_cast<int32>(EKey::S)))
        m_targetPosition += m_editorCameraEntity->GetBackward() * p_deltaTime * m_moveSpeed;

    if (ImGui::IsKeyDown(static_cast<int32>(EKey::A)))
        m_targetPosition += m_editorCameraEntity->GetLeft() * p_deltaTime * m_moveSpeed;
    else if (ImGui::IsKeyDown(static_cast<int32>(EKey::D)))
        m_targetPosition += m_editorCameraEntity->GetRight() * p_deltaTime * m_moveSpeed;

    if (ImGui::IsKeyDown(static_cast<int32>(EKey::Q)))
        m_targetPosition += m_editorCameraEntity->GetDown() * p_deltaTime * m_moveSpeed;
    else if (ImGui::IsKeyDown(static_cast<int32>(EKey::E)))
        m_targetPosition += m_editorCameraEntity->GetUp() * p_deltaTime * m_moveSpeed;
}