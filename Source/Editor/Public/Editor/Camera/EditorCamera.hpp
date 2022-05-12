#ifndef __EDITOR_CAMERA_HPP__
#define __EDITOR_CAMERA_HPP__

#include "GameFramework/Entity.hpp"
#include "Camera/CameraComponent.hpp"
#include "World.hpp"

class EditorCamera
{
    public:

    // ============================== [Public Constructor and Destructor] ============================== //

        EditorCamera    ();

        EditorCamera    (EditorCamera const&)   = default;

        EditorCamera    (EditorCamera&&)        = default;

        ~EditorCamera   ()                      = default;
        
    // ============================== [Public Local Methods] ============================== //

        [[nodiscard]]
        CameraComponent*    GetEditorCamera         ()                                  noexcept;

        [[nodiscard]]
        CameraComponent*    GetLevelCamera          ()                                  noexcept;

        void                SetLevelCamera          (CameraComponent*   p_camera)       noexcept;

        void                Tick                    (float const        p_deltaTime)    noexcept;

        [[nodiscard]]
        float               GetRotationSpeed        ()                                  const noexcept;

        [[nodiscard]]
        float               GetMovementSpeed        ()                                  const noexcept;

        void                SetRotationSpeed        (float const        p_speed)        noexcept;

        void                SetMovementSpeed        (float const        p_speed)        noexcept;

        void                SetFocus                (Vector3 const&     p_position)     noexcept;

        [[nodiscard]]
        float               GetFocusSpeed           ()                                  const noexcept;

        void                SetFocusSpeed           (float const        p_speed)        noexcept;

        [[nodiscard]]
        float               GetFocusDistance        ()                                  const noexcept;

        void                SetFocusDistance        (float const        p_distance)     noexcept;

    // ============================== [Public Local Operators] ============================== //

        EditorCamera&   operator=   (EditorCamera const&)   = default;

        EditorCamera&   operator=   (EditorCamera&&)        = default;

    private:

    // ============================== [Private Local Properties] ============================== //

        Entity*             m_editorCameraEntity;

        CameraComponent*    m_editorCameraComponent;

        CameraComponent*    m_levelCamera;

        ImVec2              m_dragDelta;

        Vector3             m_targetPosition;

        Vector3             m_targetRotation;

        float               m_rotationSpeed;

        float               m_moveSpeed;

        World*              m_world;

        bool                m_isFocusing;

        Vector3             m_focusStart;

        Vector3             m_focusEnd;

        float               m_focusTime;

        float               m_focusDistance;

        float               m_focusSpeed;

    // ============================== [Private Local Methods] ============================== //

        void    UpdateDragDelta ()                              noexcept;

        void    UpdatePosition  (float const    p_deltaTime)    noexcept;

        void    UpdateRotation  (float const    p_deltaTime)    noexcept;

        void    UpdateCamera    ()                              noexcept;

        void    ResetTarget     ()                              noexcept;

        void    ApplyFocus      (float const    p_deltaTime)    noexcept;
};

#include "EditorCamera.inl"

#endif // !__EDITOR_CAMERA_HPP__