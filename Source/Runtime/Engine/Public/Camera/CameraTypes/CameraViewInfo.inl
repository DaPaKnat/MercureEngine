#ifndef __CAMERA_VIEW_INFO_INL__
#define __CAMERA_VIEW_INFO_INL__

//=========================[Public Constructor + Destructor]=========================//

FORCEINLINE CameraViewInfo::CameraViewInfo  ()
:   m_location              {Vector3::Zero},
    m_rotation              {Quaternion::Identity},
    m_fieldOfView           {90.0f},
    m_orthoWidth            {512.0f},
    m_nearClipPlane         {0.1f},
    m_farClipPlane          {1000.0f},
    m_aspectRatio           {16.0f/9.0f},
    m_constrainAspectRatio  {false},
    m_projectionMode        {ECameraProjectionMode::Perspective}
{}

FORCEINLINE CameraViewInfo::CameraViewInfo  (Vector3 const&                 p_location,
                                             Quaternion const&              p_rotation,
                                             float const                    p_fieldOfView,
                                             float const                    p_orthoWidth,
                                             float const                    p_nearClipPlane,
                                             float const                    p_farClipPlane,
                                             float const                    p_aspectRatio,
                                             bool const                     p_constrainAspectRatio,
                                             ECameraProjectionMode const    p_projectionMode)
:   m_location              {p_location},
    m_rotation              {p_rotation},
    m_fieldOfView           {p_fieldOfView},
    m_orthoWidth            {p_orthoWidth},
    m_nearClipPlane         {p_nearClipPlane},
    m_farClipPlane          {p_farClipPlane},
    m_aspectRatio           {p_aspectRatio},
    m_constrainAspectRatio  {p_constrainAspectRatio},
    m_projectionMode        {p_projectionMode}
{}


FORCEINLINE CameraViewInfo::CameraViewInfo  (CameraViewInfo const&          p_copy)
:   m_location              {p_copy.m_location},
    m_rotation              {p_copy.m_rotation},
    m_fieldOfView           {p_copy.m_fieldOfView},
    m_orthoWidth            {p_copy.m_orthoWidth},
    m_nearClipPlane         {p_copy.m_nearClipPlane},
    m_farClipPlane          {p_copy.m_farClipPlane},
    m_aspectRatio           {p_copy.m_aspectRatio},
    m_constrainAspectRatio  {p_copy.m_constrainAspectRatio},
    m_projectionMode        {p_copy.m_projectionMode}
{}

FORCEINLINE CameraViewInfo::CameraViewInfo  (CameraViewInfo&&               p_move)
:   m_location              {std::move(p_move.m_location)},
    m_rotation              {std::move(p_move.m_rotation)},
    m_fieldOfView           {std::move(p_move.m_fieldOfView)},
    m_orthoWidth            {std::move(p_move.m_orthoWidth)},
    m_nearClipPlane         {std::move(p_move.m_nearClipPlane)},
    m_farClipPlane          {std::move(p_move.m_farClipPlane)},
    m_aspectRatio           {std::move(p_move.m_aspectRatio)},
    m_constrainAspectRatio  {std::move(p_move.m_constrainAspectRatio)},
    m_projectionMode        {std::move(p_move.m_projectionMode)}
{}

//=========================[Public Local Operators]=========================//


FORCEINLINE bool                CameraViewInfo::operator==  (CameraViewInfo const&  p_other)    const
{
    return  m_location == p_other.m_location                            &&     
            m_rotation == p_other.m_rotation                            &&
            m_fieldOfView == p_other.m_fieldOfView                      &&
            m_orthoWidth == p_other.m_orthoWidth                        &&
            m_nearClipPlane == p_other.m_nearClipPlane                  &&
            m_farClipPlane == p_other.m_farClipPlane                    &&
            m_aspectRatio == p_other.m_aspectRatio                      &&
            m_constrainAspectRatio == p_other.m_constrainAspectRatio    &&
            m_projectionMode == p_other.m_projectionMode;
}

FORCEINLINE bool                CameraViewInfo::operator!=  (CameraViewInfo const&  p_other)    const
{
    return !operator==(p_other);
}

FORCEINLINE CameraViewInfo&     CameraViewInfo::operator=   (CameraViewInfo const&  p_copy)
{
    m_location              = p_copy.m_location;
    m_rotation              = p_copy.m_rotation;
    m_fieldOfView           = p_copy.m_fieldOfView;
    m_orthoWidth            = p_copy.m_orthoWidth;
    m_nearClipPlane         = p_copy.m_nearClipPlane;
    m_farClipPlane          = p_copy.m_farClipPlane;
    m_aspectRatio           = p_copy.m_aspectRatio;
    m_constrainAspectRatio  = p_copy.m_constrainAspectRatio;
    m_projectionMode        = p_copy.m_projectionMode;

    return *this;
}

FORCEINLINE CameraViewInfo&     CameraViewInfo::operator=   (CameraViewInfo&&       p_move)
{
    m_location              = std::move(p_move.m_location);
    m_rotation              = std::move(p_move.m_rotation);
    m_fieldOfView           = std::move(p_move.m_fieldOfView);
    m_orthoWidth            = std::move(p_move.m_orthoWidth);
    m_nearClipPlane         = std::move(p_move.m_nearClipPlane);
    m_farClipPlane          = std::move(p_move.m_farClipPlane);
    m_aspectRatio           = std::move(p_move.m_aspectRatio);
    m_constrainAspectRatio  = std::move(p_move.m_constrainAspectRatio);
    m_projectionMode        = std::move(p_move.m_projectionMode);

    return *this;
}

#endif // !__CAMERA_VIEW_INFO_INL__