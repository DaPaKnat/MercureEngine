#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include "CoreMinimal.hpp"
#include "ObjectManager.hpp"

#include "Components/SceneComponent.hpp"

// ==============================[Forward Declaration]============================== //

    class Level;
    class World;

// ================================================================================= //

class ENGINE_API Entity : public Object
{
    REFLECT(Entity)

    public:

    // =================================[Public Constructors and Destructor]================================= //

        Entity  ();

        Entity  (Entity const&   p_copy)    = default;

        Entity  (Entity&&        p_move)    = default;

        ~Entity ()                          = default;

    // =================================[Public Local Methods]================================= //
        
        template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<EntityComponent, T>>>
        T*                                                  AddComponent                (std::string const&             p_name = "",
                                                                                         Args...                        p_args)
        {
            T* component = ObjectManager::Get().CreateObject<T>(p_name, p_args...);

            if (component != nullptr)
            {
                component->SetOwner(this);

                if (m_hasBegunPlay)
                {
                    component->PreInitialize();
                    component->Initialize();
                    component->PostInitialize();
                }

                m_ownedComponents.push_back(component);
            }

            return component;
        }

        bool                                                AddComponent                (EntityComponent*               p_component);

        void                                                AddLocalOffset              (Vector3 const&                 p_deltaLocation,
                                                                                         bool const                     p_sweep = false, 
                                                                                         HitResult*                     p_outHitResult = nullptr);

        void                                                AddLocalRotation            (Quaternion const&              p_deltaRotation,
                                                                                         bool const                     p_sweep = false, 
                                                                                         HitResult*                     p_outHitResult = nullptr);

        void                                                AddLocalTransform           (Transform const&               p_deltaTransform,
                                                                                         bool const                     p_sweep = false, 
                                                                                         HitResult*                     p_outHitResult = nullptr);

        void                                                AddWorldOffset              (Vector3 const&                 p_deltaLocation,
                                                                                         bool const                     p_sweep = false, 
                                                                                         HitResult*                     p_outHitResult = nullptr);

        void                                                AddWorldRotation            (Quaternion const&              p_deltaRotation,
                                                                                         bool const                     p_sweep = false, 
                                                                                         HitResult*                     p_outHitResult = nullptr);
        
        void                                                AddWorldTransform           (Transform const&               p_deltaTransform,
                                                                                         bool const                     p_sweep = false, 
                                                                                         HitResult*                     p_outHitResult = nullptr);
        
		virtual void										BeginPlay					();

        bool                                                CanEverTick                 ()                                                      const;

        void                                                CanEverTick                 (bool const                     p_canEverTick);

        void                                                Deserialize                 (Json const&                    p_deserialize,
                                                                                         Json const&                    p_components)          override;

        bool                                                Destroy                     ();

        void                                                DispatchBlockingHit         (PrimitiveComponent* const      p_myComponent,
                                                                                         PrimitiveComponent* const      p_otherComponent,
                                                                                         bool const                     p_selfMoved,
                                                                                         HitResult const&               p_hitResult);

		virtual void										EndPlay						();

        FORCEINLINE Vector3                                 GetBackward                 ()                                                      const;

        FORCEINLINE std::vector<EntityComponent*> const&    GetComponents               ()                                                      const;

        template <typename Type> 
        void                                                GetComponents               (std::vector<Type*>&            p_outEntityComp)        const;

        template <typename Type>
        void                                                GetComponents               (std::vector<Type>&             p_outEntityComp)        const;

        void                                                GetComponents               (std::vector<EntityComponent*>& p_outEntityComp)        const;

        FORCEINLINE float                                   GetDistanceTo               (Entity const*                  p_other)                const;

        FORCEINLINE float                                   GetDistanceSquaredTo        (Entity const*                  p_other)                const;

        FORCEINLINE Vector3                                 GetDown                     ()                                                      const;

        FORCEINLINE bool                                    GetEnableCollision          ()                                                      const;

        FORCEINLINE Vector3                                 GetForward                  ()                                                      const;    

        FORCEINLINE Vector3                                 GetLeft                     ()                                                      const;

//      Level*                                              GetLevel                    ()                                                      const;

        FORCEINLINE Vector3                                 GetRelativeScale            ()                                                      const;

        FORCEINLINE Vector3                                 GetRight                    ()                                                      const;

        FORCEINLINE SceneComponent*                         GetRootComponent            ()                                                      const;

        FORCEINLINE Vector3                                 GetUp                       ()                                                      const;                                 
         
        World*                                              GetWorld                    ()                                                      const;

        FORCEINLINE Vector3                                 GetWorldLocation            ()                                                      const;

        FORCEINLINE Quaternion                              GetWorldRotation            ()                                                      const;

        FORCEINLINE Vector3                                 GetWorldScale               ()                                                      const;

        FORCEINLINE Transform                               GetWorldTransform           ()                                                      const;

        bool                                                HasBegunPlay                ()                                                      const;

        bool                                                HasValidRootComponent       ()                                                      const;

        void                                                Initialize                  ();

        bool                                                IsInitialized               ()                                                      const;

        bool                                                IsOverlapping               (Entity const*                  p_other)                const;

        bool                                                IsOwning                    (EntityComponent*               p_entityComponent)      const;

        virtual void                                        NotifyBeginOverlap          (Entity const*                  p_otherEntity);

        virtual void                                        NotifyEndOverlap            (Entity const*                  p_otherEntity);

        virtual void                                        NotifyHit                   (PrimitiveComponent const*      p_myComponent,
                                                                                         Entity const*                  p_otherEntity,
                                                                                         PrimitiveComponent const*      p_otherComponent,
                                                                                         bool const                     p_selfMoved,
                                                                                         Vector3 const&                 p_hitLocation,
                                                                                         Vector3 const&                 p_hitNormal,
                                                                                         Vector3 const&                 p_normalImpulse,
                                                                                         HitResult const&               p_hitResult);

        void                                                PreInitialize               ();

        void                                                PostInitialize              ();

        bool                                                RemoveComponent             (EntityComponent*               p_entityComponent);

        void                                                Serialize                   (Json&                          p_save)                 const override;

        void                                                SetEnableCollision          (bool const                     p_newEnableCollision);

        void                                                SetRelativeLocation         (Vector3 const&                 p_newRelativeLocation,
                                                                                         bool const                     p_sweep = false, 
                                                                                         HitResult*                     p_outHitResult = nullptr);
        
        void                                                SetRelativeRotation         (Quaternion const&              p_newRelativeRotation,
                                                                                         bool const                     p_sweep = false, 
                                                                                         HitResult*                     p_outHitResult = nullptr);
        
        void                                                SetRelativeScale            (Vector3 const&                 p_newRelativeScale);
        
        void                                                SetRelativeTransform        (Transform const&               p_newRelativeTransform,
                                                                                         bool const                     p_sweep = false, 
                                                                                         HitResult*                     p_outHitResult = nullptr);
        
        bool                                                SetRootComponent            (SceneComponent*                p_newRootComponent);

        void                                                SetWorldLocation            (Vector3 const&                 p_newLocation,
                                                                                         bool const                     p_sweep = false, 
                                                                                         HitResult*                     p_outHitResult = nullptr);
        
        void                                                SetWorldLocationAndRotation (Vector3 const&                 p_newLocation, 
                                                                                         Quaternion const&              p_newRotation,
                                                                                         bool const                     p_sweep = false, 
                                                                                         HitResult*                     p_outHitResult = nullptr);
        
        void                                                SetWorldRotation            (Quaternion const&              p_newRotation,
                                                                                         bool const                     p_sweep = false, 
                                                                                         HitResult*                     p_outHitResult = nullptr);
        
        void                                                SetWorldScale               (Vector3 const&                 p_newScale);
        
        void                                                SetWorldTransform           (Transform const&               p_newTransform,
                                                                                         bool const                     p_sweep = false, 
                                                                                         HitResult*                     p_outHitResult = nullptr);

        void                                                SetWorld                    (World*                         p_world);

		virtual void								        Tick						(float const					p_deltaTime);

        void                                                UpdateComponentTransforms   ();
        
    // =================================[Public Local Operators]================================= //

        Entity& operator=   (Entity const&) = default;

        Entity& operator=   (Entity&&)      = default;

    protected:
        
    // =================================[Protected Local Properties]================================= //

		PROPERTY()
        /** Should we tick this actor. */
        bool    m_canEverTick;

    // =================================[Protected Local Methods]================================= //

        virtual void    OnPreInitialize     ();

        virtual void    OnInitialize        ();

        virtual void    OnPostInitialize    ();

        virtual void    OnDestroy           ();

    private:

    // =================================[Private Local Properties]================================= //

        /** 
         * The root components of this entity. 
         * If it's different of nullptr, this entity is spatialized, otherwise it's not. 
         */
        SceneComponent*                 m_rootComponent;

        /** All entity components ownn by this entity. */
        std::vector<EntityComponent*>   m_ownedComponents;

        /** ... */
        bool                            m_hasBegunPlay;

        /** ... */
		PROPERTY()
        bool                            m_enableCollision;

        World*                          m_world;

    // =================================[Private Local Methods]================================= //

        void    InternalDispatchBlockingHit (PrimitiveComponent* const      p_myComponent,
                                             PrimitiveComponent* const      p_otherComponent,
                                             bool const                     p_selfMoved,
                                             HitResult const&               p_hitResult); 

}; // !class Entity

#include "Entity.inl"
#include "Entity.generated.hpp"

#endif // !__ENTITY_HPP_