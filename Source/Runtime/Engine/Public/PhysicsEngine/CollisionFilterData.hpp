#ifndef __COLLISION_FILTER_DATA_HPP__
#define __COLLISION_FILTER_DATA_HPP__

#include "CoreMinimal.hpp"

struct CollisionFilterData
{

    // ==============================[Public Local Properties]============================== //

        /** ... */
        uint32  m_word0;

        /** ... */
        uint32  m_word1;

        /** ... */
        uint32  m_word2;

        /** ... */
        uint32  m_word3;

    // ==============================[Public Local Constructor + Destructor]============================== //

        CollisionFilterData     ();

        CollisionFilterData     (uint32 const               p_word0,
                                 uint32 const               p_word1,
                                 uint32 const               p_word2,
                                 uint32 const               p_word3);

        CollisionFilterData     (CollisionFilterData const& p_copy);

        CollisionFilterData     (CollisionFilterData&&      p_move);

        ~CollisionFilterData    ()                                      = default;


    // ==============================[Public Local Operators]============================== //

        bool                    operator==  (CollisionFilterData const& p_other)    const;
        bool                    operator!=  (CollisionFilterData const& p_other)    const;

        CollisionFilterData&    operator=   (CollisionFilterData const& p_copy);
        CollisionFilterData&    operator=   (CollisionFilterData&&      p_move);

};  // !struct CollisionFilterData

#endif // !__COLLISION_FILTER_DATA_HPP__