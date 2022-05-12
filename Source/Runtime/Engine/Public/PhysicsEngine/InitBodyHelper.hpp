#ifndef __INIT_BODY_HELPER_HPP__
#define __INIT_BODY_HELPER_HPP__

#include "CoreMinimal.hpp"

// ==============================[Foward Declaraction]============================== //

class BodyInstance;
class PrimitiveComponent;
class PhysicsSceneHandle;

// ==================================================================================//

/**
 * Description : TODO !
 */
class InitBodyHelper
{
    public:

    // ==============================[Public Constructor + Destructor]============================== //

        InitBodyHelper  () = default;

        InitBodyHelper  (BodyInstance*          p_bodyInstance,
                         Transform const&       p_transform,
                         PrimitiveComponent*    p_primitiveComponent,
                         PhysicsSceneHandle*    p_physicsScene);

        InitBodyHelper  (InitBodyHelper const&  p_copy) = default;

        InitBodyHelper  (InitBodyHelper&&       p_move) = default;

        ~InitBodyHelper  () = default;

    // ==============================[Public Constructor + Destructor]============================== //

        void    InitBody    ();

    private:

    // ==============================[Private Local Properties]============================== //

        /** The body instance that we want to create. */
        BodyInstance*       m_bodyInstance;

        /** The primitive component that the body instance is part of. */
        PrimitiveComponent* m_primitiveComponent;

        /** The location and rotation where we spawn the body. */
        Transform           m_transform;
        
        /** The corresponding scene where we want add this body.*/
        PhysicsSceneHandle* m_physicsSceneHandle;


    // ==============================[Private Local Methods]============================== //
    
        void    CreateActor         ();

        bool    CreateActorAndShape ();

        bool    CreateShape         ();

        bool    IsStatic            ();


}; // !class InitBodyHelper

#endif // !__INIT_BODY_HELPER_HPP__