#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "GameFramework/Entity.hpp"

#include "Components/CapsuleComponent.hpp"
#include "Components/StaticMeshComponent.hpp"
#include "Camera/CameraComponent.hpp"

// ==============================[Forward Declaration]============================== //

class PlayerMovementComponent;
class PlayerController;
enum class EKey;
enum class EState;
enum class EButton;

// ================================================================================= //

DECLARE_LOG_CATEGORY(LogPlayer);

class ENGINE_API Player : public Entity
{
    REFLECT(Player)

    typedef Entity Super;

    public:

    // ==============================[Public Local Constructor]============================== //

        Player  ();

        Player  (Player const&  p_copy) = default;

        Player  (Player&&       p_move) = default;

        ~Player ()  = default;

    // ==============================[Virtual Public Local Methods]============================== //

        virtual void    BeginPlay   ()  override;

    // ==============================[Public Local Methods]============================== //

        void                AddMovementInput        (Vector3 const& p_worldDirection,
                                                    float const    p_scaleValue);

        void                AddMovementInput        (Vector3 const& p_worldAcceleration);

        Vector3             ConsumeMovementInput    ();

        CapsuleComponent*   GetCapsuleCollider      ()  const;

        void                Jump                    ();

        void                StopJumping             ();

    // ==============================[Public Local Operators]============================== //

        Player& operator=   (Player const&  p_copy) = default;
        
        Player& operator=   (Player&&       p_move) = default;

    protected:

    // ==============================[Protected Local Properties]============================== //

        CapsuleComponent*           m_capsuleCollider           = nullptr;

        StaticMeshComponent*        m_rifleMesh                 = nullptr;

        SceneComponent*             m_rifleMuzzle               = nullptr;

        CameraComponent*            m_playerCamera              = nullptr;

        PlayerMovementComponent*    m_playerMovementComponent   = nullptr;

        PlayerController*           m_playerController          = nullptr;

        Vector3                     m_movementInput             = Vector3::Zero;

        Vector3                     m_lastMovementInput         = Vector3::Zero;
        
    private:

    // ==============================[Private Local Properties]============================== //

        bool    ComponentsCreation              ();

        void    InitializeCapsuleCollider       ();

        void    InitializePlayerCamera          ();

        void    InitializeProperties            ();

        void    InitializeRifleMesh             ();

        void    InitializeRifleMuzzle           ();

}; // !class Player

#include "Player.inl"

#include "Player.generated.hpp"

#endif // !__PLAYER_HPP__
