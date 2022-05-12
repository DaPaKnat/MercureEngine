#ifndef __COLLISION_RESPONSE_CONTAINER_HPP__
#define __COLLISION_RESPONSE_CONTAINER_HPP__

#include "CoreMinimal.hpp"

#include "EngineTypes.hpp"

/**
 * Container for indicating a set of collision channels that this object will collide with.
 */
class ENGINE_API CollisionResponseContainer
{
    public:

    // ==============================[Public Static Methods]==============================//

        FORCEINLINE static CollisionResponseContainer const&    GetDefaultResponseContainer();

    // ==============================[Public Local Properties]==============================//

        union
        {
            struct
            {
              
                // Reserved Engine Trace Channel //
                uint8   m_worldStatic;  // 0
                uint8   m_worldDynamic; // 1
            
                // Unspecified Game Trace Channels //
			    uint8   m_gameTraceChannel1;     // 2
			    uint8   m_gameTraceChannel2;     // 3
			    uint8   m_gameTraceChannel3;     // 4
			    uint8   m_gameTraceChannel4;     // 5
			    uint8   m_gameTraceChannel5;     // 6
			    uint8   m_gameTraceChannel6;     // 7
			    uint8   m_gameTraceChannel7;     // 8
			    uint8   m_gameTraceChannel8;     // 9
			    uint8   m_gameTraceChannel9;     // 10
			    uint8   m_gameTraceChannel10;    // 11
			    uint8   m_gameTraceChannel11;    // 12
			    uint8   m_gameTraceChannel12;    // 13
			    uint8   m_gameTraceChannel13;    // 14
			    uint8   m_gameTraceChannel14;    // 15
			    uint8   m_gameTraceChannel15;    // 16
			    uint8   m_gameTraceChannel16;    // 17
			    uint8   m_gameTraceChannel17;    // 18
                uint8   m_gameTraceChannel18;    // 19
                uint8   m_gameTraceChannel19;    // 20
                uint8   m_gameTraceChannel20;    // 21
                uint8   m_gameTraceChannel21;    // 22
                uint8   m_gameTraceChannel22;    // 23
                uint8   m_gameTraceChannel23;    // 24
                uint8   m_gameTraceChannel24;    // 25
                uint8   m_gameTraceChannel25;    // 26
                uint8   m_gameTraceChannel26;    // 27
                uint8   m_gameTraceChannel27;    // 28
                uint8   m_gameTraceChannel28;    // 29
                uint8   m_gameTraceChannel29;    // 30
                uint8   m_gameTraceChannel30;    // 31
            };
            
            uint8   m_enumArray[32];
        };

    // ==============================[Public Constructor + Destructor]==============================//
    
        CollisionResponseContainer  ();

        CollisionResponseContainer  (ECollisionResponse const           p_defaultResponse);

        CollisionResponseContainer  (CollisionResponseContainer const&  p_copy);

        CollisionResponseContainer  (CollisionResponseContainer&&       p_move);

        ~CollisionResponseContainer ()  = default;

    // ==============================[Public Local Methods]==============================//

        FORCEINLINE ECollisionResponse  GetResponse     (ECollisionChannel const    p_channel)      const;
        
        void                            ReplaceChannels (ECollisionResponse const   p_oldResponse,
                                                         ECollisionResponse const   p_newResponse);

        void                            SetAllChannels  (ECollisionResponse const   p_newResponse);
    
        void                            SetResponse     (ECollisionChannel const    p_channel,
                                                         ECollisionResponse const   p_newResonse);

    // ==============================[Public Local Operators]==============================//

        bool                        operator==  (CollisionResponseContainer const&  p_other)    const;
        bool                        operator!=  (CollisionResponseContainer const&  p_other)    const;

        CollisionResponseContainer& operator=   (CollisionResponseContainer const&  p_copy);
        CollisionResponseContainer& operator=   (CollisionResponseContainer&&       p_move);

    private:

    // ==============================[Private Static Properties]==============================//

        /** Static variable for default data */
        static CollisionResponseContainer DefaultResponseContainer;
};

#include "CollisionResponseContainer.inl"

#endif // !__COLLISION_RESPONSE_CONTAINER_HPP__