#ifndef __PHYSICS_ACTOR_CREATION_PARAMETERS_HPP__
#define __PHYSICS_ACTOR_CREATION_PARAMETERS_HPP__

#include "CoreMinimal.hpp"

#include "PhysicsSceneHandle.hpp"

struct PhysicsActorCreationParameters
{
    // ==============================[Public Local Properties]============================== //

        /** The physics scene used to create the actor */
        PhysicsSceneHandle*     m_physicsScene;

        /** The transform of the actor*/
        Transform               m_transform;

        /** ... */
        bool                    m_isStatic;

        /** ... */
        bool                    m_queryOnly;

        /** ... */
        bool                    m_enableGravity;

}; // ! struct PhysicsActorCreationParameters

#endif // !__PHYSICS_ACTOR_CREATION_PARAMETERS_HPP__
