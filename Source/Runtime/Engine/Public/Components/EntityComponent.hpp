#ifndef __ENTITY_COMPONENT_HPP__
#define __ENTITY_COMPONENT_HPP__

#include "CoreMinimal.hpp"

#include "EngineTypes.hpp"

// ==============================[Forward Declaration]============================== //

    class Entity;
    class World;

// ================================================================================= //

class ENGINE_API EntityComponent : public Object
{
    REFLECT(EntityComponent)

    public:

    // =================================[Public Constructor + Destructor]================================= //
    
        EntityComponent     ();

        EntityComponent     (EntityComponent const&  p_copy)    = default;

        EntityComponent     (EntityComponent&&       p_move)    = default;

        ~EntityComponent    ()                                  = default;

    // =================================[Virtual Public Local Methods]================================= //

        virtual	void	Activate			            ();

		virtual void	BeginPlay			            ();

		virtual void	Deactivate			            ();

        virtual void    Deserialize                     (Json const&    p_deseriliaze,
                                                         Json const&    p_components)
        {
            *this = p_deseriliaze;
        }

		virtual void	EndPlay				            ();

        virtual bool    HasValidPhysicsState            ()                              const;

		virtual	bool	IsActive			            ();

        virtual void    OnCreatePhysicsState            ();

        virtual void    OnDestroyPhysicsState           ();
        
        virtual void    OnEntityEnableCollisionChanged  ();

        virtual void    Serialize                       (Json&          p_serialize)    const override;

		virtual void	SetActive			            (bool const		                p_newActive);

		virtual	bool	ShouldActivate	                ()  const;

        virtual bool    ShouldCreatePhysicsState        ()  const;

		virtual void	Tick			                (float const	                p_deltaTime);

		virtual void	ToggleActive		            ();

        virtual void    UpdateLocalToWorld              (EUpdateTransformFlags const    p_updateTransformFlags = EUpdateTransformFlags::None, 
                                                         ETeleportType const            p_teleportType = ETeleportType::None);

    // =================================[Public Local Methods]================================= //

        void                CanEverTick             (bool const p_canEverTick);

        bool                CanEverTick             ()                          const;

        void                CreatePhysicsState      ();

        void                DestroyPhysicsState     ();

        bool                Destroy                 ();

		FORCEINLINE Entity*	GetOwner			    ()                          const;

        World*              GetWorld                ()                          const;

        FORCEINLINE bool    HasBegunPlay            ()                          const;

        void                Initialize              ();

        bool                IsInitialized           ()                          const;

        FORCEINLINE bool    IsPhysicsStateCreated   ()                          const;

        void                PostInitialize          ();

        void                PreInitialize           ();

        void                RecreatePhysicsState    ();

        void                SetOwner                (Entity*    p_owner);

    // =================================[Public Local Operators]================================= //

        EntityComponent&    operator=   (EntityComponent const&) = default;

    protected:

    // =================================[Protected Local Properties]================================= //

        /** ... */
        bool    m_physicsStateCreated;

        bool    m_hasBegunPlay;

        bool    m_isInitialized;

    // =================================[Protected Local Methods]================================= //

        virtual void    OnDestroy           ();

        virtual void    OnInitialize        ();

        virtual void    OnPostInitialize    ();

        virtual void    OnPreInitialize     ();

    private:

    // =================================[Private Local Properties]================================= //

        /** ... */
		PROPERTY()
        bool    m_canEverTick;

        /** ... */
        bool    m_isActive;

        /** ... */
        Entity* m_owner;

}; // ! class EntityComponent

#include "EntityComponent.inl"
#include "EntityComponent.generated.hpp"

#endif // !__ENTITY_COMPONENT_HPP__