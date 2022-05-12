#ifndef __MOVING_PLATFORM_HPP__
#define __MOVING_PLATFORM_HPP__

#include "CoreMinimal.hpp"

#include "Wall.hpp"

class ENGINE_API MovingPlatform : public Wall
{
    REFLECT(MovingPlatform)

    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        MovingPlatform  ();

        MovingPlatform  (MovingPlatform const&) = default;

        MovingPlatform  (MovingPlatform&&)      = default;

        ~MovingPlatform ()                      = default;

    // ============================== [Public Local Methods] ============================== //

        virtual void    BeginPlay       ()                              override;

        virtual void    Tick            (float const    p_deltaTime)    override;

        virtual void    EndPlay         ()                              override;

        void            SetSpeed        (float const    p_speed) { m_speed = p_speed; }

        void            SetDirection    (Vector3 const& p_vector) { m_direction = p_vector; }

        void            SetDistance     (float const    p_dist) { m_distance = p_dist; }

    // ============================== [Public Local Operators] ============================== //

        MovingPlatform& operator=   (MovingPlatform const&) = default;

        MovingPlatform& operator=   (MovingPlatform&&)      = default;

    private:

        PROPERTY() 
        float   m_speed         = 0.2f;

        PROPERTY()
        Vector3 m_direction     = Vector3::Up;

        PROPERTY()
        float   m_distance      = 28.5f;

        PROPERTY()
        float   m_waitingTime   = 1.0f;

        Vector3 m_origin        = Vector3::Zero;

        float   m_time          = 0.0f;

        float   m_check         = 0.0f;
};

#include "MovingPlatform.generated.hpp"

#endif // !__HORIZONTAL_PLATFORM_HPP__