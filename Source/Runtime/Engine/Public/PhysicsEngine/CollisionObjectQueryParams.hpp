#ifndef __COLLISION_OBJECT_QUERY_PARAMS_HPP__
#define __COLLISION_OBJECT_QUERY_PARAMS_HPP__

#include "EngineTypes.hpp"

/**
 * TODO : Description
 */
class ENGINE_API CollisionObjectQueryParams
{
    public :

    // ==============================[Public Static Properties]============================== //
        
        static CollisionObjectQueryParams   DefaultObjectQueryParams;

    // ==============================[Public Local Constructor + Destructor]============================== //

        CollisionObjectQueryParams  ();

        CollisionObjectQueryParams  (ECollisionChannel const                p_queryChannel);

        CollisionObjectQueryParams  (std::vector<ECollisionChannel> const&  p_objectTypes);

        CollisionObjectQueryParams  (CollisionObjectQueryParams const&      p_copy) = default;

        CollisionObjectQueryParams  (CollisionObjectQueryParams&&           p_move) = default;

        ~CollisionObjectQueryParams () = default;

    // ==============================[Public Local Methods]============================== //

        void    AddObjectTypeToQuery    (ECollisionChannel const    p_objectTypeToQuery);

        int32   GetQueryBitfield        ()  const;

        bool    IsValid                 ()  const;

        void    RemoveObjectTypeToQuery (ECollisionChannel const    p_objectTypeToQuery);

        void    SetQueryBitfield        (int32 const                p_objectTypeToQuery);

    // ==============================[Public Local Operators]============================== //

        bool                        operator==  (CollisionObjectQueryParams const&  p_other)  const;
        bool                        operator!=  (CollisionObjectQueryParams const&  p_other)  const;

        CollisionObjectQueryParams& operator=   (CollisionObjectQueryParams const&  p_copy);
        CollisionObjectQueryParams& operator=   (CollisionObjectQueryParams&&       p_move);

    private:

    // ==============================[Private Local Properties]============================== //

        /** ... */
        int32   m_objectTypeToQuery;

}; // !class CollisionObjectQueryParams

#endif // !__COLLISION_OBJECT_QUERY_PARAMS_HPP__