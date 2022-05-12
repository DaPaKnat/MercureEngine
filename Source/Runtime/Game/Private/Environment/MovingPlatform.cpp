#include "PCH.hpp"

#include "Environment/MovingPlatform.hpp"

MovingPlatform::MovingPlatform  () :
    Wall()
{

}

void    MovingPlatform::BeginPlay   ()
{
    Wall::BeginPlay();

    m_origin = GetWorldLocation();
}

void    MovingPlatform::Tick        (float const    p_deltaTime)
{
    Entity::Tick(p_deltaTime);

    if (m_waitingTime != 0.0 && (Vector3::DistanceSquared(GetWorldLocation(), m_origin) <= 0.005f ||
        Vector3::DistanceSquared(GetWorldLocation(), m_origin + m_direction.GetUnsafeNormalized() * m_distance) <= 0.005f))
    {
        m_check += p_deltaTime;

        if (m_check >= m_waitingTime)
            m_check = 0.0f;
        else
            return;
    }

    m_time += p_deltaTime * m_speed;

    if (m_direction != Vector3::Zero)
        SetWorldLocation(Vector3::Lerp(m_origin, m_origin + m_direction.GetUnsafeNormalized() * m_distance, Math::PingPong(m_time, 1)));
}

void    MovingPlatform::EndPlay   ()
{
    Wall::EndPlay();

    SetWorldLocation(m_origin);

    m_time = m_check = 0.0f;
}