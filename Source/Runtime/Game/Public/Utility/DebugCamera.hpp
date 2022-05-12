#ifndef __DEBUG_CAMERA_HPP__
#define __DEBUG_CAMERA_HPP__

#include "CoreMinimal.hpp"

#include "GameFramework/Entity.hpp"
#include "Camera/CameraComponent.hpp"
#include "Light/PointLightComponent.hpp"
#include "Application.hpp"

class ENGINE_API DebugCamera : public Entity
{
    REFLECT(DebugCamera)

    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        DebugCamera ();

        DebugCamera  (DebugCamera const&) = default;

        DebugCamera  (DebugCamera&&) = default;

        ~DebugCamera () = default;

        // ============================== [Public Local Methods] ============================== //

        virtual void    BeginPlay   ()                              override;

        virtual void    Tick        (float const    p_deltaTime)    override;

        virtual void    EndPlay     ()                              override;

        // ============================== [Public Local Operators] ============================== //

        DebugCamera& operator=   (DebugCamera const&) = default;

        DebugCamera& operator=   (DebugCamera&&) = default;

    private:

        PROPERTY()
        CameraComponent*        m_camera            = nullptr;
        
        PROPERTY()
        PointLightComponent*    m_light             = nullptr;
        PROPERTY()
        float                   m_moveSpeed         = 15.0f;

        PROPERTY()
        float                   m_rotationSpeed     = 5.0f;

        Vector3                 m_targetPosition    = Vector3::Zero;

        Vector3                 m_targetRotation    = Vector3::Zero;

        bool                    m_rotating          = false;

        Vector2                 m_dragDelta         = Vector2::Zero;

        bool                    left, right, up, down, forward, back;

        void                KeyboardInputHandler    (EKey const     p_key,
                                                     EState const   p_state);

        void                MouseInputHandler       (EButton const  p_button,
                                                     EState const   p_state);
};

#include "DebugCamera.generated.hpp"

#endif // !__DEBUG_CAMERA_HPP__