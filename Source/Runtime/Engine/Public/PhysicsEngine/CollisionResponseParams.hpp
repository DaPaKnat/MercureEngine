#ifndef __COLLISION_RESPONSE_PARAMS__
#define __COLLISION_RESPONSE_PARAMS__

#include "CoreMinimal.hpp"

#include "CollisionResponseContainer.hpp"


/**
 * TODO : Description
 */
class ENGINE_API CollisionResponseParams
{
    public:

    // ==============================[Public Static Properties]============================== //

        /** ... */
        static CollisionResponseParams  DefaultResponseParams;

    // ==============================[Public Local Properties]============================== //

        CollisionResponseContainer  m_collisionResponseContainer;

    // ==============================[Public Local Constructor + Destructor ]============================== //

        CollisionResponseParams     (ECollisionResponse const           p_defaultResponse = ECollisionResponse::Block);

        CollisionResponseParams     (CollisionResponseContainer const&  p_responseContainer);

        CollisionResponseParams     (CollisionResponseParams const&     p_copy);

        CollisionResponseParams     (CollisionResponseParams&&          p_move);

        ~CollisionResponseParams    () = default;
    
    // ==============================[Public Local Operators]============================== //

        bool                        operator==  (CollisionResponseParams const& p_other)    const;

        bool                        operator!=  (CollisionResponseParams const& p_other)    const;

        CollisionResponseParams&    operator=   (CollisionResponseParams const& p_copy);  

        CollisionResponseParams&    operator=   (CollisionResponseParams&&      p_move);

}; // !class CollisionResponseParams

#endif // !__COLLISION_RESPONSE_PARAMS__
