#ifndef __CAMERA_COMPONENT_INL__
#define __CAMERA_COMPONENT_INL__

// ==============================[Public Virtual Local Methods]============================== //

INLINE  void    CameraComponent::Serialize   (Json&  p_serialize)    const
{
    p_serialize = Json(*this);

    SceneComponent::SerializeHierarchy(p_serialize);
}

// ==============================[Public Local Methods]============================== //

FORCEINLINE float       CameraComponent::GetAdditiveFOVOffset   ()  const
{
    return m_additiveFOVOffset;
}

FORCEINLINE Transform   CameraComponent::GetAdditiveOffset      ()  const
{
    return m_additiveOffset;
}


#endif // !__CAMERA_COMPONENT_INL__