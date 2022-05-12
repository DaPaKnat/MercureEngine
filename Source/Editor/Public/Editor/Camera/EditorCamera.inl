#ifndef __EDITOR_CAMERA_INL__
#define __EDITOR_CAMERA_INL__

// ============================== [Public Local Methods] ============================== //

INLINE CameraComponent* EditorCamera::GetEditorCamera   ()                              noexcept
{
    return m_editorCameraComponent;
}

INLINE CameraComponent* EditorCamera::GetLevelCamera    ()                              noexcept
{
    return m_levelCamera;
}
 
INLINE void             EditorCamera::SetLevelCamera    (CameraComponent*   p_camera)   noexcept
{
    m_levelCamera = p_camera;
}

INLINE float            EditorCamera::GetMovementSpeed  ()                              const noexcept
{
    return m_moveSpeed;
}

INLINE float            EditorCamera::GetRotationSpeed  ()                              const noexcept
{
    return m_rotationSpeed;
}

INLINE void             EditorCamera::SetMovementSpeed  (float const        p_speed)     noexcept
{
    m_moveSpeed = p_speed;
}

INLINE void             EditorCamera::SetRotationSpeed  (float const        p_speed)     noexcept
{
    m_rotationSpeed = p_speed;
}

INLINE void             EditorCamera::SetFocus          (Vector3 const&     p_position) noexcept
{
    if (m_world && !m_world->HasBegunPlay())
    {
        m_focusStart    = m_editorCameraComponent->GetWorldLocation();
        m_focusEnd      = p_position + m_editorCameraComponent->GetBackward() * m_focusDistance;
        m_isFocusing    = true;
    }
}

INLINE void             EditorCamera::SetFocusSpeed     (float const        p_speed)    noexcept
{                                             
    m_focusSpeed = p_speed;
}                                             
                                              
INLINE float            EditorCamera::GetFocusSpeed     ()                              const noexcept
{
    return m_focusDistance;
}

INLINE void             EditorCamera::SetFocusDistance  (float const        p_distance) noexcept
{
    m_focusDistance = p_distance;
}

INLINE float            EditorCamera::GetFocusDistance  ()                              const noexcept
{
    return m_focusDistance;
}

// ============================== [Private Local Methods] ============================== //

INLINE void             EditorCamera::UpdateDragDelta   ()                              noexcept
{
    m_dragDelta = ImGui::GetMouseDragDelta(1);
}

INLINE void             EditorCamera::UpdateRotation    (float const    p_deltaTime)    noexcept
{
    m_targetRotation = m_editorCameraEntity->GetWorldTransform().m_rotation.GetEulerDeg();

    m_targetRotation.m_y += m_dragDelta.x * p_deltaTime * m_rotationSpeed;
    m_targetRotation.m_x += m_dragDelta.y * p_deltaTime * m_rotationSpeed;
}

INLINE void             EditorCamera::UpdateCamera      ()                              noexcept
{
    m_editorCameraEntity->SetWorldLocation(m_editorCameraEntity->GetWorldLocation() + m_targetPosition);
    m_editorCameraEntity->SetWorldRotation(Quaternion::MakeFromEulerDeg(m_targetRotation));
}

INLINE void             EditorCamera::ResetTarget       ()                              noexcept
{
    m_targetPosition = m_targetRotation = Vector3::Zero;
    ImGui::ResetMouseDragDelta(1);
}

INLINE void             EditorCamera::ApplyFocus        (float const    p_deltaTime)    noexcept
{
    m_focusTime = Math::Clamp01(m_focusTime + p_deltaTime * m_focusSpeed);

    m_editorCameraComponent->SetWorldLocation(Vector3::Lerp(m_focusStart, m_focusEnd, m_focusTime));

    if (m_focusTime >= 1.0f)
    {
        m_isFocusing = false;
        m_focusTime = 0.0f;
    }
}

#endif // !__EDITOR_CAMERA_INL__