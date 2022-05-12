#ifndef __SCENE_COMPONENT_HPP__
#define __SCENE_COMPONENT_HPP__

#include "EntityComponent.hpp"

#include "PhysicsEngine/CollisionResponseContainer.hpp"
#include "PhysicsEngine/OverlapInfo.hpp"

class ENGINE_API SceneComponent : public EntityComponent
{
    REFLECT(SceneComponent)

    // =================================[Typedef]================================= //
        
        typedef EntityComponent Super;

    public:

    // =================================[Public Constructor + Destructor]================================= //

        SceneComponent    ();

        SceneComponent    (SceneComponent const&  p_copy)   = default;

        SceneComponent    (SceneComponent&&       p_move)   = default;

        ~SceneComponent   ()                                = default;

    // =================================[Virtual Public Local Methods]================================= //

        virtual bool                                CanAttachAsChild                (SceneComponent const*      p_child)        const;

        virtual void                                Deserialize                     (Json const&    p_deserialize,
                                                                                     Json const&    p_components)
        {
            *this = p_deserialize;

            if (p_deserialize.contains("Parent"))
            {
                Json node = p_deserialize["Parent"];

                m_attachParent = static_cast<SceneComponent*>(ObjectManager::Get().GetObjectOfType(node["TypeID"].get<size_t>(), node["ID"].get<size_t>()));
            }

            if (p_deserialize.contains("Childrens"))
            {
                for (auto const& node : p_deserialize["Childrens"])
                {
                    SceneComponent* children = static_cast<SceneComponent*>(ObjectManager::Get().GetObjectOfType(node["TypeID"].get<size_t>(), node["ID"].get<size_t>()));

                    if (children)
                        m_attachChildrens.push_back(children);
                }
            }
        }

        virtual ECollisionEnabled                   GetCollisionEnabled             ()                                          const;

        virtual ECollisionChannel                   GetCollisionObjectType          ()                                          const;

        virtual ECollisionResponse                  GetCollisionResponseToChannel   (ECollisionChannel const    p_channel)      const;
        
        virtual CollisionResponseContainer const&   GetCollisionResponseToChannels  ()                                          const;

        virtual bool                                IsSimulatingPhysics             ()                                          const;

        FORCEINLINE virtual bool                    IsVisible                       ()                                          const;

        virtual void                                OnAttachementChanged            ();

        virtual void                                OnChildAttached                 (SceneComponent*            p_child);

        virtual void                                OnChildDetached                 (SceneComponent*            p_child);

        virtual void                                Serialize                       (Json&                      p_serialize)    const override;

        virtual void                                OnUpdateTransform               (EUpdateTransformFlags const        p_updateTransformFlags,
                                                                                     ETeleportType const                p_teleportType = ETeleportType::None);

        virtual void                                SetMobility                     (EComponentMobility const   p_newMobility);

        virtual void                                UpdateLocalToWorld              (EUpdateTransformFlags      p_updateTransformFlags = EUpdateTransformFlags::None, 
                                                                                     ETeleportType const        p_teleportType = ETeleportType::None) override final;


    // =================================[Public Local Methods]================================= //

        void                                AddLocalOffset                  (Vector3 const&                     p_deltaLocalLocation,
                                                                             bool const                         p_sweep = false, 
                                                                             HitResult*                         p_outHitResult = nullptr);

        void                                AddLocalRotation                (Quaternion const&                  p_deltaLocalRotation,
                                                                             bool const                         p_sweep = false, 
                                                                             HitResult*                         p_outHitResult = nullptr);

        void                                AddLocalTransform               (Transform const&                   p_deltaLocalTransform,
                                                                             bool const                         p_sweep = false, 
                                                                             HitResult*                         p_outHitResult = nullptr);

        void                                AddRelativeOffset               (Vector3 const&                     p_deltaRelativeLocation,
                                                                             bool const                         p_sweep = false, 
                                                                             HitResult*                         p_outHitResult = nullptr);

        void                                AddRelativeRotation             (Quaternion const&                  p_deltaRelativeRotation,
                                                                             bool const                         p_sweep = false, 
                                                                             HitResult*                         p_outHitResult = nullptr);
      
        void                                AddWorldOffset                  (Vector3 const&                     p_deltaWorldLocation,
                                                                             bool const                         p_sweep = false, 
                                                                             HitResult*                         p_outHitResult = nullptr);

        void                                AddWorldRotation                (Quaternion const&                  p_deltaWorldRotation,
                                                                             bool const                         p_sweep = false, 
                                                                             HitResult*                         p_outHitResult = nullptr);

        void                                AddWorldTransform               (Transform const&                   p_deltaTransform,
                                                                             bool const                         p_sweep = false, 
                                                                             HitResult*                         p_outHitResult = nullptr);

        bool                                AttachTo                        (SceneComponent*                    p_parent,
                                                                             AttachmentTransformRules const&    p_attachmentRules);

        FORCEINLINE void                    ConditionUpdateLocalToWorld     ();

        void                                Detach                          (DetachmentTransformRules const&    p_rules);


        std::vector<SceneComponent*> const& GetAttachChildrens              ()                                                      const;

        SceneComponent*                     GetAttachRoot                   ()                                                      const;

        Entity*                             GetAttachRootEntity             ();

        FORCEINLINE SceneComponent*         GetAttachParent                 ()                                                      const;

        FORCEINLINE Vector3                 GetBackward                     ()                                                      const;

        FORCEINLINE SceneComponent*         GetChildren                     (int32 const                        p_childrenIndex)    const;

        FORCEINLINE int32                   GetChildrenCount                ()                                                      const;

        void                                GetChildrens                    (bool const                         p_includeAllDescendants,
                                                                             std::vector<SceneComponent*>&      p_childrens);
        
        ECollisionResponse                  GetCollisionResponseToComponent (SceneComponent const*              p_otherComponent)   const;

        FORCEINLINE Vector3                 GetDown                         ()                                                      const;

        FORCEINLINE Vector3                 GetForward                      ()                                                      const;

        FORCEINLINE Vector3                 GetLeft                         ()                                                      const;

        FORCEINLINE EComponentMobility      GetMobility                     ()                                                      const;

        void                                GetParents                      (std::vector<SceneComponent*>&      p_parents)          const;

        FORCEINLINE Vector3                 GetRelativeLocation             ()                                                      const;
        
        FORCEINLINE Quaternion              GetRelativeRotation             ()                                                      const;

        FORCEINLINE Vector3                 GetRelativeScale                ()                                                      const;

        FORCEINLINE Transform               GetRelativeTransform            ()                                                      const;

        FORCEINLINE Vector3                 GetRight                        ()                                                      const;
        
        FORCEINLINE Vector3                 GetUp                           ()                                                      const;

        FORCEINLINE Vector3                 GetWorldLocation                ()                                                      const;

        FORCEINLINE Quaternion              GetWorldRotation                ()                                                      const;

        FORCEINLINE Vector3                 GetWorldScale                   ()                                                      const;

        FORCEINLINE Transform const&        GetWorldTransform               ()                                                      const; 


        bool                                IsAttachedTo                    (SceneComponent const*              p_testComponent)    const;

        FORCEINLINE bool                    IsCollisionEnabled              ()                                                      const;

        FORCEINLINE bool                    IsQueryCollisionEnabled         ()                                                      const;

        FORCEINLINE bool                    IsPhysicsCollisionEnabled       ()                                                      const;

        FORCEINLINE bool                    MoveComponent                   (Vector3 const&                     p_deltaWorldOffset,
                                                                             Quaternion const&                  p_newWorldRotation,
                                                                             bool const                         p_sweep, 
                                                                             HitResult*                         p_outHitResult = nullptr);

        void                                ResetRelativeTransform          ();

        FORCEINLINE void                    SetRelativeLocation             (Vector3 const&                     p_newRelativeLocation,
                                                                             bool const                         p_sweep = false, 
                                                                             HitResult*                         p_outHitResult = nullptr);

        FORCEINLINE void                    SetRelativeLocationAndRotation  (Vector3 const&                     p_newRelativeLocation,
                                                                             Quaternion const&                  p_newRelativeRotation,
                                                                             bool const                         p_sweep = false, 
                                                                             HitResult*                         p_outHitResult = nullptr);

        FORCEINLINE void                    SetRelativeRotation             (Quaternion const&                  p_newRelativeRotation,
                                                                             bool const                         p_sweep = false, 
                                                                             HitResult*                         p_outHitResult = nullptr);

        void                                SetRelativeScale                (Vector3 const&                     p_newRelativeScale);

        void                                SetRelativeTransform            (Transform const&                   p_newRelativeTransform,
                                                                             bool const                         p_sweep = false, 
                                                                             HitResult*                         p_outHitResult = nullptr);

        void                                SetupAttachment                 (SceneComponent*                    p_inParent);

        void                                SetVisibilityInGame             (bool const                         p_newVisibility,
                                                                             bool const                         p_propagateToChildren = false);

        void                                SetWorldLocation                (Vector3 const&                     p_newWorldLocation,
                                                                             bool const                         p_sweep = false, 
                                                                             HitResult*                         p_outHitResult = nullptr);

        void                                SetWorldLocationAndRotation     (Vector3 const&                     p_newWorldLocation,
                                                                             Quaternion const&                  p_newWorldRotation,
                                                                             bool const                         p_sweep = false, 
                                                                             HitResult*                         p_outHitResult = nullptr);

        void                                SetWorldRotation                (Quaternion const&                  p_newWorldRotation,
                                                                             bool const                         p_sweep = false, 
                                                                             HitResult*                         p_outHitResult = nullptr);

        void                                SetWorldScale                   (Vector3 const&                     p_newWorldScale);

        void                                SetWorldTransform               (Transform const&                   p_newWorldTransform,
                                                                             bool const                         p_sweep = false, 
                                                                             HitResult*                         p_outHitResult = nullptr);

        FORCEINLINE void                    ToggleVisibilityInGame          (bool const                         p_propagateToChildren = false);

        void                                UpdateChildTransforms           (EUpdateTransformFlags const        p_updateTransformFlags = EUpdateTransformFlags::None, 
                                                                             ETeleportType                      p_teleport = ETeleportType::None);

        void                                UpdateOverlaps                  (std::vector<OverlapInfo> const*    p_pendingOverlaps = nullptr,
                                                                             bool const                         p_doNotifies = true,
                                                                             std::vector<OverlapInfo> const*    p_overlapAtEndLocation = nullptr);

        void                                SerializeHierarchy              (Json&                              p_json) const;

    // =================================[Public Local Operators]================================= //

        SceneComponent&   operator=   (SceneComponent const&) = default;

    protected:

    // =================================[Protected Local Properties]================================= //

        /** ... */
        EComponentMobility  m_mobility;

        /** 
         * The transform corresponding to the location, rotation and scale in world space. 
         * The matrix of this one corresponds to the local to world matrix.
         */
        Transform           m_localToWorld;

        /** If true, OnUpdateTransform virtual will be called each time this component is moved. */
        bool                m_wantsOnUpdateTransform;

    // =================================[Virtual Protected Local Methods]================================= //

        virtual bool    MoveComponentImpl           (Vector3 const&                     p_deltaWorldOffset,
                                                     Quaternion const&                  p_newWorldRotation,
                                                     bool const                         p_sweep, 
                                                     HitResult*                         p_outHitResult = nullptr);

        virtual void    OnVisibilityInGameChanged   ();

        virtual void    UpdateOverlapsImpl          (std::vector<OverlapInfo> const*    p_pendingOverlaps = nullptr,
                                                     bool const                         p_doNotifies = true,
                                                     std::vector<OverlapInfo> const*    p_overlapAtEndLocation = nullptr);

    // =================================[Protected Local Methods]================================= //

        FORCEINLINE Transform   ComputeNewLocalToWorld              (Transform const&       p_newRelativeTransform,
                                                                     SceneComponent const*  p_parent = nullptr)     const;

        bool                    InternalSetWorldLocationAndRotation (Vector3 const&         p_newWorldLocation,
                                                                     Quaternion const&      p_newWorldRotation);

    private:

    // =================================[Private Local Properties]================================= //

        /** 
         * The parent scene component of this one.
         * If m_attachParnet is nullptr, so this component have not parent.
         */
        SceneComponent*                 m_attachParent;

        /** 
         * The childrens scene component of this one.
         */
        std::vector<SceneComponent*>    m_attachChildrens;

        /** Should this component is visible in game. */
        PROPERTY()
        bool                            m_isVisibleInGame;

        /** Is the local to world transform of this scene component is updated. */
        bool                            m_isLocalToWorldUpdated;
        
        /**
         * The transform corresponding to the location, rotation and scale in relative space.
         * The local space correspond to the local to world transform of its parent. 
         * If it has not parent, so the relative transform is equal to the local to world transform.
         * The matrix of this one corresponds to the local to world matrix.
         */
        PROPERTY()
        Transform                       m_relativeTransform;

    // =================================[Private Local Methods]================================= //
    
        void    GetChildrenWithDescendant       (std::vector<SceneComponent*>&  p_childrenWithDescendant);

        void    PropagateTransformUpdate        (bool const                     p_transformChanged,
                                                 EUpdateTransformFlags const    p_updateTransformFlags = EUpdateTransformFlags::None,
                                                 ETeleportType const            p_teleport = ETeleportType::None);

        void    UpdateLocalToWorldWithParent    (SceneComponent*                m_attachParent,
                                                 EUpdateTransformFlags const    p_updateTransformFlags,
                                                 ETeleportType const            p_teleport = ETeleportType::None);

}; // !class SceneComponent

#include "SceneComponent.inl"
#include "SceneComponent.generated.hpp"

#endif // !__SCENE_COMPONENT_HPP__