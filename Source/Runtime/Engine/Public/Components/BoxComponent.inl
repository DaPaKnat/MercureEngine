#ifndef __BOX_COMPONENT_INL__
#define __BOX_COMPONENT_INL__

// ==============================[Public Local Methods]============================== //

/**
 * Gets the box extent WITH the component scale applied.
 *
 * @return  The scale box extent.
 */
FORCEINLINE Vector3 BoxComponent::GetScaledBoxExtent    ()  const
{
    return m_boxExtent * GetWorldTransform().m_scale;
}

/**
 * Gets the box extent WITHOUT the component scale applied.
 *
 * @return  The scale box extent.
 */
FORCEINLINE Vector3 BoxComponent::GetUnscaledBoxExtent  ()  const
{
    return m_boxExtent;
}

/**
 * Sets the box extent without triggering physics update.
 *
 * @param p_boxExtent   The new box extent.
 */
FORCEINLINE void    BoxComponent::InitBoxExtent         (Vector3 const& p_boxExtent)
{
    m_boxExtent = p_boxExtent;
}

/**
 * Sets the box extent without triggering physics update.
 *
 * @param p_boxExtentXYZ    The value of each extent axis.
 */
FORCEINLINE void    BoxComponent::InitBoxExtent         (float const    p_boxExtentXYZ)
{
    m_boxExtent.Set(p_boxExtentXYZ);
}

/**
 * Sets the box extent without triggering physics update.
 *
 * @param p_boxExtentX   The new box extent on X axis.
 * @param p_boxExtentY   The new box extent on Y axis.
 * @param p_boxExtentZ   The new box extent on Z axis.
 */
FORCEINLINE void    BoxComponent::InitBoxExtent         (float const    p_boxExtentX,
                                                         float const    p_boxExtentY,
                                                         float const    p_boxExtentZ)
{
    m_boxExtent.Set(p_boxExtentX, p_boxExtentY, p_boxExtentZ);
}

#endif // !__BOX_COMPONENT_INL__
