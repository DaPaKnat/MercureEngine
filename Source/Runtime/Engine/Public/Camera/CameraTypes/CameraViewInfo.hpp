#ifndef __CAMERA_VIEW_INFO_HPP__
#define __CAMERA_VIEW_INFO_HPP__

#include "CoreMinimal.hpp"

#include "CameraTypeEnums.hpp"

struct ENGINE_API CameraViewInfo
{

    //=========================[Public Static Properties]=========================//

        /** ... */
        Vector3                 m_location;

        /** ... */
        Quaternion              m_rotation;

        /** ... */
        float                   m_fieldOfView;

        /** ... */
        float                   m_orthoWidth;

        /** ... */
        float                   m_nearClipPlane;

        /** ... */
        float                   m_farClipPlane;

        /** ... */
        float                   m_aspectRatio;

        /** ... */
        bool                    m_constrainAspectRatio;

        /** ... */
        ECameraProjectionMode   m_projectionMode;


    //=========================[Public Constructor + Destructor]=========================//

        FORCEINLINE CameraViewInfo  ();
        
        FORCEINLINE CameraViewInfo  (Vector3 const&                 p_location,
                                     Quaternion const&              p_rotation,
                                     float const                    p_fieldOfView,
                                     float const                    p_orthoWidth,
                                     float const                    p_nearClipPlane,
                                     float const                    p_farClipPlane,
                                     float const                    p_aspectRatio,
                                     bool const                     p_constrainAspectRatio,
                                     ECameraProjectionMode const    p_projectionMode);

        FORCEINLINE CameraViewInfo  (CameraViewInfo const&          p_copy);
         
        FORCEINLINE CameraViewInfo  (CameraViewInfo&&               p_move);

                    ~CameraViewInfo ()                                  = default;

    //=========================[Public Local Methods]=========================//

        Matrix4x4   ComputeProjectionMatrix    ()  const;

    //=========================[Public Local Operators]=========================//

        FORCEINLINE bool            operator==  (CameraViewInfo const&  p_other)    const;
        FORCEINLINE bool            operator!=  (CameraViewInfo const&  p_other)    const;
         
        FORCEINLINE CameraViewInfo& operator=   (CameraViewInfo const&  p_copy);
        FORCEINLINE CameraViewInfo& operator=   (CameraViewInfo&&       p_move);

};

#include "CameraViewInfo.inl"

#endif // !__CAMERA_VIEW_INFO_HPP__
