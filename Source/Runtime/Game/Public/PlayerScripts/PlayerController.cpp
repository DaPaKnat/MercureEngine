#include "PCH.hpp"

#include "PlayerScripts/PlayerController.hpp"

#include "Application.hpp"

DEFINE_LOG_CATEGORY(LogPlayerController);

// ==============================[Virtual Public Local Methods]============================== //

void    PlayerController::BeginPlay ()
{
    Super::BeginPlay();

    SetupPlayerOwner();
    RegisterInputHandlers();
}

void    PlayerController::EndPlay   ()
{
    Super::EndPlay();

    UnregisterInputHandlers();
}

void    PlayerController::Tick      (float const    p_deltaTime)
{
    ManageMovement();
}

// ==============================[Protected Local Methods]============================== //

void    PlayerController::KeyboardInputHandler      (EKey const     p_key, 
                                                     EState const   p_state)
{
    if (m_playerOwner)
    {
        switch (p_key)
        {
            case EKey::W :
            {
                if (p_state == EState::PRESSED)
                {
                    m_movementInput.m_z += 1.0f;
                }

                else if (p_state == EState::RELEASED)
                {
                    m_movementInput.m_z -= 1.0f;
                }
                break;
            }

            case EKey::S:
            {
                if (p_state == EState::PRESSED)
                {
                    m_movementInput.m_z -= 1.0f;
                }

                else if (p_state == EState::RELEASED)
                {
                    m_movementInput.m_z += 1.0f;
                }
                break;
            }

            case EKey::A:
            {
                if (p_state == EState::PRESSED)
                {
                    m_movementInput.m_x -= 1.0f;
                }

                else if (p_state == EState::RELEASED)
                {
                    m_movementInput.m_x += 1.0f;
                }
                break;
            }

            case EKey::D:
            {
                if (p_state == EState::PRESSED)
                {
                    m_movementInput.m_x += 1.0f;
                }

                else if (p_state == EState::RELEASED)
                {
                    m_movementInput.m_x -= 1.0f;
                }
                break;
            }

            case EKey::SPACE:
            {
                if (p_state == EState::PRESSED)
                {
                    m_playerOwner->Jump();
                }

                else if (p_state == EState::RELEASED)
                {
                    m_playerOwner->StopJumping();
                }
                break;
            }
        }
    }
}

void    PlayerController::ManageMovement            ()
{
    if (!m_movementInput.IsZero() && m_playerOwner)
    {
        Vector3 const direction {m_playerOwner->GetForward() + m_playerOwner->GetRight()};
        m_playerOwner->AddMovementInput(direction * m_movementInput, 1.0f);
    }
}

void    PlayerController::MouseInputHandler         (EButton const  p_button,
                                                     EState const   p_state)
{
    if (m_playerOwner)
    {
        switch (p_button)
        {
            case EButton::MOUSE_LEFT:
            {
                if (p_state == EState::PRESSED)
                {
                    LOG(LogPlayerController, Display, "Player entity '%s' : DoSwapColorState()", GetName());
                }
                break;
            }

            case EButton::MOUSE_RIGHT :
            {
                if (p_state == EState::PRESSED)
                {
                    LOG(LogPlayerController, Display, "Player entity '%s' : DoFire()", GetName());
                }
                break;
            }
        } 
    }
}

bool    PlayerController::RegisterInputHandlers     ()
{
    bool registeringInputSucced {true};

    std::unique_ptr<Window> const&  mainWindow {Application::Get().GetMainWindow()};
    if (mainWindow)
    {
        mainWindow->BindKeyCallback<PlayerController, &PlayerController::KeyboardInputHandler>(this);
        mainWindow->BindMouseButtonCallback<PlayerController, &PlayerController::MouseInputHandler>(this);
    }

    else
    {
        registeringInputSucced = false;
        LOG(LogPlayerController, Error, "Player Controller '%s' : Registering input handlers failed !", GetName());
    }

    return registeringInputSucced;
}

bool    PlayerController::SetupPlayerOwner          ()
{
    m_playerOwner = dynamic_cast<Player*>(GetOwner());
    if (m_playerOwner == nullptr)
    {
        LOG(LogPlayerController, Error, "Trying to setup a nullptr player owner.");
        return false;
    }

    return true;
}

bool    PlayerController::UnregisterInputHandlers   ()
{
    bool unregisteringInputSucced   {true};

    std::unique_ptr<Window> const&  mainWindow {Application::Get().GetMainWindow()};
    if (mainWindow)
    {
        mainWindow->RemoveKeyCallback<PlayerController, &PlayerController::KeyboardInputHandler>(this);
        mainWindow->RemoveMouseButtonCallback<PlayerController, &PlayerController::MouseInputHandler>(this);
    }

    else
    {
        unregisteringInputSucced = false;
        LOG(LogPlayerController, Error, "Player Controller '%s' : Registering input handlers failed !", GetName());
    }

    return unregisteringInputSucced;
}