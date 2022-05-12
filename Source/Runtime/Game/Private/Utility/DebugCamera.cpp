#include "PCH.hpp"

#include "Utility/DebugCamera.hpp"
#include "World.hpp"

DebugCamera::DebugCamera    () :
    Entity()
{
    m_camera = AddComponent<CameraComponent>("Camera");

    SetRootComponent(m_camera);

    m_camera->m_fieldOfView = 45.0f;
    m_camera->m_aspectRatio = static_cast<float>(Application::Get().GetMainWindow()->GetExtent().width) /
                              static_cast<float>(Application::Get().GetMainWindow()->GetExtent().height); 
    
    SetWorldTransform(Matrix4x4::LookAt(m_camera->GetWorldLocation(), Vector3::Forward * 5, Vector3::Up));

    m_light = AddComponent<PointLightComponent>("Light");

    m_light->SetupAttachment(m_camera);

    m_light->SetRelativeLocation(Vector3::Forward * 2);
}

void    DebugCamera::BeginPlay()
{
    Entity::BeginPlay();

    if (World* world = GetWorld())
        world->SetMainCamera(m_camera);

    Application::Get().GetMainWindow()->BindKeyCallback<DebugCamera, &DebugCamera::KeyboardInputHandler>(this);
    Application::Get().GetMainWindow()->BindMouseButtonCallback<DebugCamera, &DebugCamera::MouseInputHandler>(this);

    left = right = up = down = forward = back = false;
}

void    DebugCamera::EndPlay()
{
    Entity::EndPlay();

    Application::Get().GetMainWindow()->RemoveKeyCallback<DebugCamera, &DebugCamera::KeyboardInputHandler>(this);
    Application::Get().GetMainWindow()->RemoveMouseButtonCallback<DebugCamera, &DebugCamera::MouseInputHandler>(this);
}

void    DebugCamera::Tick   (float const p_deltaTime)
{
    Entity::Tick(p_deltaTime);

    if (m_rotating)
    {
        Vector2 drag = Application::Get().GetMainWindow()->GetCursorPosition() - m_dragDelta;

        m_targetRotation = GetWorldTransform().m_rotation.GetEulerDeg();

        m_targetRotation.m_y += drag.m_x * p_deltaTime * m_rotationSpeed;
        m_targetRotation.m_x += drag.m_y * p_deltaTime * m_rotationSpeed;
        
        m_dragDelta = Application::Get().GetMainWindow()->GetCursorPosition();
    }

    if (forward)
        m_targetPosition += GetForward() * m_moveSpeed;
    else if (back)
        m_targetPosition += GetBackward() * m_moveSpeed;

    if (left)
        m_targetPosition += GetLeft() * m_moveSpeed;
    else if (right)
        m_targetPosition += GetRight() * m_moveSpeed;

    if (down)
        m_targetPosition += GetDown() * m_moveSpeed;
    else if (up)
        m_targetPosition += GetUp() * m_moveSpeed;

    SetWorldLocation(GetWorldLocation() + m_targetPosition * p_deltaTime);
    SetWorldRotation(Quaternion::MakeFromEulerDeg(m_targetRotation));

    m_targetPosition = Vector3::Zero;
}

void    DebugCamera::KeyboardInputHandler      (EKey const     p_key,
                                                EState const   p_state)
{
    switch (p_key)
    {
        case EKey::W:
        {
            if (p_state == EState::PRESSED)
                forward = true;
            else if (p_state == EState::RELEASED)
                forward = false;

            break;
        }
    
        case EKey::S:
        {
            if (p_state == EState::PRESSED)
                back = true;
            else if (p_state == EState::RELEASED)
                back = false;

            break;
        }
    
        case EKey::A:
        {
            if (p_state == EState::PRESSED)
                left = true;
            else if (p_state == EState::RELEASED)
                left = false;

            break;
        }
    
        case EKey::D:
        {
            if (p_state == EState::PRESSED)
                right = true;
            else if (p_state == EState::RELEASED)
                right = false;

            break;
        }

        case EKey::Q:
        {
            if (p_state == EState::PRESSED)
                down = true;
            else if (p_state == EState::RELEASED)
                down = false;

            break;
        }

        case EKey::E:
        {
            if (p_state == EState::PRESSED)
                up = true;
            else if (p_state == EState::RELEASED)
                up = false;

            break;
        }
    }
}

void    DebugCamera::MouseInputHandler          (EButton const  p_button,
                                                 EState const   p_state)
{
    switch (p_button)
    {
        case EButton::MOUSE_RIGHT:
        {
            if (p_state == EState::PRESSED)
            {
                m_rotating = true;
                m_dragDelta = Application::Get().GetMainWindow()->GetCursorPosition();
            }
            else if (p_state == EState::RELEASED)
                m_rotating = false;

            break;
        }
    }
}