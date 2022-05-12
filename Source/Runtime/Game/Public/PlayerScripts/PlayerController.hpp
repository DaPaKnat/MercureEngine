#ifndef __PLAYER_CONTROLLER_HPP__
#define __PLAYER_CONTROLLER_HPP__

#include "Components/EntityComponent.hpp"

#include "Player.hpp"

DECLARE_LOG_CATEGORY(LogPlayerController);

// ==============================[Forward Declaration]============================== //

enum class EKey;
enum class EState;
enum class EButton;

// ================================================================================= //

class ENGINE_API PlayerController : public EntityComponent
{
    typedef EntityComponent Super;
    
    public :
    
    // ==============================[Public Local Constructor]============================== //

        PlayerController    () = default;

        PlayerController    (PlayerController const&    p_copy) = default;

        PlayerController    (PlayerController&&         p_move) = default;

        ~PlayerController   () = default;

    // ==============================[Virtual Public Local Methods]============================== //

        virtual void    BeginPlay   ()  override;

        virtual void    EndPlay     ()  override;

        virtual void    Tick        (float const    p_deltaTime)    override;

    // ==============================[Public Local Operators]============================== //

        PlayerController&   operator=   (PlayerController const&  p_copy) = default;
        
        PlayerController&   operator=   (PlayerController&&       p_move) = default;

    protected :

    // ==============================[Protected Local Properties]============================== //

        Player* m_playerOwner               = nullptr;

        Vector3 m_movementInput             = Vector3::Zero;

        Vector3 m_rotationInput             = Vector3::Zero;

    // ==============================[Protected Local Methods]============================== //

        void    KeyboardInputHandler    (EKey const     p_key, 
                                         EState const   p_state);

        void    ManageMovement          ();

        void    MouseInputHandler       (EButton const  p_button,
                                         EState const   p_state);

        bool    RegisterInputHandlers   ();

        bool    SetupPlayerOwner        ();

        bool    UnregisterInputHandlers ();

}; // !class PlayerController

#endif // !__PLAYER_CONTROLLER_HPP__
