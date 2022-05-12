#ifndef __WORLD_HPP__
#define __WORLD_HPP__

#include "CoreMinimal.hpp"
#include "ObjectManager.hpp"
#include "PhysicsEngine/PhysicsSceneHandle.hpp"

// ==============================[Forward Declaration]============================== //

	struct	GameWorldSettings;
	class	Entity;
	class	Level;
    class   CameraComponent;

// ================================================================================= //

class ENGINE_API World
{
	public:

	// ============================== [Public Constructors and Destructor] ============================== //

		World   ();

		~World	() = default;

	// ============================== [Public Local Methods] ============================== //

        Level*                              CreateLevel                     (std::string const&             p_levelName,
                                                                             bool const                     p_isCurrent = false);

        Level*						        GetCurrentLevel		            ();

        PhysicsSceneHandle*                 GetPhysicsScene                 ()  const;

        void                                SetPhysicsScene                 (PhysicsSceneHandle*            p_scene);

        bool                                SaveAll                         ();

        Level*                              LoadLevel                       (std::string const& p_levelName);

        bool                                SaveCurrentLevel                ();

        void                                SetCurrentLevel                 (Level*                         p_level);

        Level*                              OpenLevel                       (std::string const& p_levelName);

        [[nodiscard]]
        GameWorldSettings*	                GetWorldSettings	            ();

		void								SetGravity			            (Vector3 const&		            p_gravity);

		Vector3								GetGravity			            ()									    const;

		Vector3								GetDefaultGravity	            ()									    const;
		
		bool								HasBegunPlay		            ()									    const;

		void								BeginPlay			            ();

		void								Tick				            (float const		            p_deltaTime);

		void								EndPlay				            ();

        bool								DestroyEntity		            (Entity*			            p_entity);

		template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<Entity, T>>>
		T*									SpawnEntity			            (std::string const&	            p_name = "",
																             Args...			            p_args)
		{
            if (m_currentLevel != nullptr)
            {
                return m_currentLevel->SpawnEntity<T>(p_name, p_args...);
            }
            else
                return nullptr;
		}

		template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<Entity, T>>>
		T*									SpawnEntity			            (Transform const&	            p_transform,
																             Args...			            p_args)
		{
            if (m_currentLevel != nullptr)
            {
                return m_currentLevel->SpawnEntity<T>(p_transform, p_args...);
            }
            else
                return nullptr;
		}

		template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<Entity, T>>>
		T*									SpawnEntity			            (std::string const&	            p_name,
																             Transform const&	            p_transform,
																             Args...			            p_args)
		{
            if (m_currentLevel != nullptr)
            {
                return m_currentLevel->SpawnEntity<T>(p_name, p_transform, p_args...);
            }
            else
                return nullptr;
		}

		size_t								GetEntityCount		            ()  const;

        void                                InitializeLevel                 ();

        void                                SetMainCamera                   (CameraComponent*               p_newMainCamera);

        CameraComponent*                    GetMainCamera                   ()  const;

        bool                                LineTraceMultiByChannel         (std::vector<HitResult>&            p_outHitResults,
                                                                             Vector3 const&                     p_traceStart,
                                                                             Vector3 const&                     p_traceEnd,
                                                                             ECollisionChannel const            p_traceChannel,
                                                                             CollisionQueryParams const&        p_queryParams = CollisionQueryParams::DefaultQueryParams,
                                                                             CollisionResponseParams const&     p_responseParams = CollisionResponseParams::DefaultResponseParams)  const;

        bool                                LineTraceMultiByObjectType      (std::vector<HitResult>&            p_outHitResults,
                                                                             Vector3 const&                     p_traceStart,
                                                                             Vector3 const&                     p_traceEnd,
                                                                             CollisionObjectQueryParams const&  p_objectQueryParams,
                                                                             CollisionQueryParams const&        p_queryParams = CollisionQueryParams::DefaultQueryParams)   const;


        bool                                LineTraceSingleByChannel        (HitResult&                         p_outHitResult,
                                                                             Vector3 const&                     p_traceStart,
                                                                             Vector3 const&                     p_traceEnd,
                                                                             ECollisionChannel const            p_traceChannel,
                                                                             CollisionQueryParams const&        p_queryParams = CollisionQueryParams::DefaultQueryParams,
                                                                             CollisionResponseParams const&     p_responseParams = CollisionResponseParams::DefaultResponseParams)  const;

        bool                                LineTraceSingleByObjectType     (HitResult&                         p_outHitResult,
                                                                             Vector3 const&                     p_traceStart,
                                                                             Vector3 const&                     p_traceEnd,
                                                                             CollisionObjectQueryParams const&  p_objectQueryParams,
                                                                             CollisionQueryParams const&        p_queryParams = CollisionQueryParams::DefaultQueryParams)  const;

        bool                                LineTraceTestByChannel          (Vector3 const&                     p_traceStart,
                                                                             Vector3 const&                     p_traceEnd,
                                                                             ECollisionChannel const            p_traceChannel,
                                                                             CollisionQueryParams const&        p_queryParams = CollisionQueryParams::DefaultQueryParams,
                                                                             CollisionResponseParams const&     p_responseParams = CollisionResponseParams::DefaultResponseParams)  const;

        bool                                LineTraceTestByObjectType       (Vector3 const&                     p_traceStart,
                                                                             Vector3 const&                     p_traceEnd,
                                                                             CollisionObjectQueryParams const&  p_objectQueryParams,
                                                                             CollisionQueryParams const&        p_queryParams = CollisionQueryParams::DefaultQueryParams)  const;

        bool                                OverlapBlockingTestByChannel    (Vector3 const&                     p_overlapLocation, 
                                                                             Quaternion const&                  p_overlapRotation, 
                                                                             ECollisionChannel const            p_traceChannel, 
                                                                             CollisionShape const&              p_collisionShape, 
                                                                             CollisionQueryParams const&        p_queryParams = CollisionQueryParams::DefaultQueryParams, 
                                                                             CollisionResponseParams const&     p_responseParams = CollisionResponseParams::DefaultResponseParams) const;
        
        
        bool                                OverlapAnyTestByChannel         (Vector3 const&                     p_overlapLocation, 
                                                                             Quaternion const&                  p_overlapRotation, 
                                                                             ECollisionChannel const            p_traceChannel, 
                                                                             CollisionShape const&              p_collisionShape, 
                                                                             CollisionQueryParams const&        p_queryParams = CollisionQueryParams::DefaultQueryParams, 
                                                                             CollisionResponseParams const&     p_responseParams = CollisionResponseParams::DefaultResponseParams) const;

        bool                                OverlapBlockingTestByObjectType (Vector3 const&                     p_overlapLocation, 
                                                                             Quaternion const&                  p_overlapRotation, 
                                                                             CollisionObjectQueryParams&        p_objectQueryParams, 
                                                                             CollisionShape const&              p_collisionShape, 
                                                                             CollisionQueryParams const&        p_queryParams = CollisionQueryParams::DefaultQueryParams) const;
        
        
        bool                                OverlapAnyTestByObjectType      (Vector3 const&                     p_overlapLocation, 
                                                                             Quaternion const&                  p_overlapRotation, 
                                                                             CollisionObjectQueryParams&        p_objectQueryParams, 
                                                                             CollisionShape const&              p_collisionShape, 
                                                                             CollisionQueryParams const&        p_queryParams = CollisionQueryParams::DefaultQueryParams)    const;

        bool                                OverlapMutliByChannel           (std::vector<OverlapResult>&        p_outOverlapResults,
                                                                             Vector3 const&                     p_overlapLocation, 
                                                                             Quaternion const&                  p_overlapRotation, 
                                                                             ECollisionChannel const            p_traceChannel, 
                                                                             CollisionShape const&              p_collisionShape, 
                                                                             CollisionQueryParams const&        p_queryParams = CollisionQueryParams::DefaultQueryParams, 
                                                                             CollisionResponseParams const&     p_responseParams = CollisionResponseParams::DefaultResponseParams)  const;

        bool                                OverlapMutliByObjectType        (std::vector<OverlapResult>&        p_outOverlapResults,
                                                                             Vector3 const&                     p_overlapLocation, 
                                                                             Quaternion const&                  p_overlapRotation, 
                                                                             CollisionObjectQueryParams&        p_objectQueryParams, 
                                                                             CollisionShape const&              p_collisionShape, 
                                                                             CollisionQueryParams const&        p_queryParams = CollisionQueryParams::DefaultQueryParams) const;

        bool                                SweepMultiByChannel             (std::vector<HitResult>&            p_outHitResults,
                                                                             Vector3 const&                     p_sweepStart, 
                                                                             Vector3 const&                     p_sweepEnd, 
                                                                             Quaternion const&                  p_rotation, 
                                                                             ECollisionChannel                  p_traceChannel, 
                                                                             CollisionShape const&              p_collisionShape, 
                                                                             CollisionQueryParams const&        p_queryParams = CollisionQueryParams::DefaultQueryParams,
                                                                             CollisionResponseParams const&     p_responseParam = CollisionResponseParams::DefaultResponseParams) const;

        bool                                SweepMultiByObjectType          (std::vector<HitResult>&            p_outHitResults,
                                                                             Vector3 const&                     p_sweepStart, 
                                                                             Vector3 const&                     p_sweepEnd, 
                                                                             Quaternion const&                  p_rotation, 
                                                                             CollisionObjectQueryParams&        p_objectQueryParams, 
                                                                             CollisionShape const&              p_collisionShape, 
                                                                             CollisionQueryParams const&        p_queryParams = CollisionQueryParams::DefaultQueryParams)   const;


        bool                                SweepSingleByChannel            (HitResult&                         p_outHitResult,
                                                                             Vector3 const&                     p_sweepStart, 
                                                                             Vector3 const&                     p_sweepEnd, 
                                                                             Quaternion const&                  p_rotation, 
                                                                             ECollisionChannel                  p_traceChannel, 
                                                                             CollisionShape const&              p_collisionShape, 
                                                                             CollisionQueryParams const&        p_queryParams = CollisionQueryParams::DefaultQueryParams,
                                                                             CollisionResponseParams const&     p_responseParam = CollisionResponseParams::DefaultResponseParams) const;

        bool                                SweepSingleByObjectType         (HitResult&                         p_outHitResult,
                                                                             Vector3 const&                     p_sweepStart, 
                                                                             Vector3 const&                     p_sweepEnd, 
                                                                             Quaternion const&                  p_rotation, 
                                                                             CollisionObjectQueryParams&        p_objectQueryParams, 
                                                                             CollisionShape const&              p_collisionShape, 
                                                                             CollisionQueryParams const&        p_queryParams = CollisionQueryParams::DefaultQueryParams)   const;



        bool                                SweepTestByChannel              (Vector3 const&                     p_sweepStart, 
                                                                             Vector3 const&                     p_sweepEnd, 
                                                                             Quaternion const&                  p_rotation, 
                                                                             ECollisionChannel                  p_traceChannel, 
                                                                             CollisionShape const&              p_collisionShape, 
                                                                             CollisionQueryParams const&        p_queryParams = CollisionQueryParams::DefaultQueryParams,
                                                                             CollisionResponseParams const&     p_responseParam = CollisionResponseParams::DefaultResponseParams) const;

        bool                                SweepTestByObjectType           (Vector3 const&                     p_sweepStart, 
                                                                             Vector3 const&                     p_sweepEnd, 
                                                                             Quaternion const&                  p_rotation, 
                                                                             CollisionObjectQueryParams&        p_objectQueryParams, 
                                                                             CollisionShape const&              p_collisionShape, 
                                                                             CollisionQueryParams const&        p_queryParams = CollisionQueryParams::DefaultQueryParams)   const;
        

    private:

	// ============================== [Private Local Properties] ============================== //

        /** The current level used by the world. */
		Level*	                m_currentLevel;


		//std::unique_ptr<ReworkPhysicsSceneHandle>	m_physicsSceneHandle;
        
        /** Has the world begin played. */
		bool			        m_hasBegunPlay;

        /** The camera component set as main camera of the world. */
        CameraComponent*        m_mainCamera;

        /** ... */
        PhysicsSceneHandle*     m_physicsScene;

}; // !class World

#include "World.inl"

#endif // !__WORLD_HPP__