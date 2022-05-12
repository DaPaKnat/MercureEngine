#ifndef __PHYSICS_ENGINE_HPP__
#define __PHYSICS_ENGINE_HPP__

#include "CoreMinimal.hpp"

#include "EngineModule.hpp"

#include "PhysXAllocatorCallback.hpp"
#include "PhysXErrorCallback.hpp"
#include "PhysXSimulationEventCallback.hpp"
#include "PhysicalMaterial.hpp"

#include "PhysicsSceneHandle.hpp"

#define DEFAULT_FIXE_DELTA_TIME 1/60.0f

class ENGINE_API PhysicsEngine final : public EngineModule
{
    public:
    
        static INLINE PhysicsEngine& Get() noexcept
        { 
            return GEngine->GetModuleManager()->Get<PhysicsEngine>();
        }

    // ==============================[Public Local Properties]============================== //

        /** PhysX Allocator Callaback*/
        PhysXAllocatorCallback          m_physXAllocatorCallback;

        /** Class that manage physX simulation event callback*/
        PhysXSimulationEventCallback    m_physicsSimulationEventCallback;

        /** Pointer to PhysX cooking object */
        physx::PxCooking*               m_physXCooking;

        /** PhysX Error Callaback */
        PhysXErrorCallback              m_physXErrorCallback;

        /** Pointer to PhysX Foundation (Singleton) */
        physx::PxFoundation*            m_physXFoundation;

        /** Pointer to PhysX SDK object */
        physx::PxPhysics*               m_physXSDK;

        /** Fixed delta time */
        float                           m_fixeDeltaTime;

        /** Max physics substep */
        uint32                          m_maxSubstep;

    // ==============================[Public Constructor + Destructor]============================== //

        PhysicsEngine   ();

        PhysicsEngine   (PhysicsEngine const&   p_copy) = default;

        PhysicsEngine   (PhysicsEngine&&        p_move) = default;

        ~PhysicsEngine  ()                              = default;

    // ============================== [Virtual Public Local Methods] ============================== //

        /** Initializes this Engine module. */
        virtual void    Initialize  (EngineKey const& p_passkey)    override;

        /** Updates this Engine module. */
        virtual void    Update      (EngineKey const& p_passkey)    override;

        /** Shutdowns this Engine module. */
        virtual void    Shutdown    (EngineKey const& p_passkey)    override;


    // ==============================  Public Local Methods] ============================== //

        PhysicsSceneHandle* GetPhysicsScene  ();

        PhysicalMaterial*   GetDefaultPhysicalMaterial  ();

    private:
        
    // ==============================[Private Local Properties]============================== //

        /** Default Physics material */
        PhysicalMaterial    m_defaultPhysicalMaterial;

        PhysicsSceneHandle  m_physicScene;

    // ==============================[Private Local Methods] ============================== //
        
        void    InitializeDefaultPhysicalMaterial   ();
        
        void    InitializePhysicsScene              ();

        void    InitalizePhysXFoundation            ();

        void    InitializePhysXCooking              ();

        void    InitializePhysXSDK                  ();

        void    ShutdownPhysicsScene                ();

        void    ShutdownPhysXFoundation             ();

        void    ShutdownPhysXCooking                ();

        void    ShutdownPhysXSDK                    ();

}; // !class PhysicsEngine

#endif // !___PHYSICS_ENGINE_HPP__