#ifndef __CAMERA_COMPONENT_HPP__
#define __CAMERA_COMPONENT_HPP__

#include "Components/SceneComponent.hpp"

#include "CameraType.hpp"

class ENGINE_API CameraComponent : public SceneComponent
{
    REFLECT(CameraComponent)

    public:

    // ==============================[Public Local Properties]============================== //

        /** ... */
        PROPERTY()
        float                   m_aspectRatio;

        /** ... */
        PROPERTY()
        bool                    m_constraintAspectRatio;

        /** ... */
        PROPERTY()
        float                   m_farClipPlane;

        /** ... */
        PROPERTY()
        float                   m_fieldOfView;

        /** ... */
        PROPERTY()
        float                   m_nearClipPlane;

        /** ... */
        PROPERTY()
        float                   m_orthoWidth;

        /** ... */
        ECameraProjectionMode   m_projectionMode;

        /** ... */
        PROPERTY()
        bool                    m_usePawnControlRotation;
        
    // ==============================[Public Constructor + Destructor]============================== //
    
        CameraComponent     ();

        CameraComponent     (CameraComponent const& p_copy) = default;

        CameraComponent     (CameraComponent&&      p_move) = default;

        ~CameraComponent    ()                              = default;

    // ==============================[Virtual Public Local Methods]============================== //

        virtual CameraViewInfo  GetCameraView       ();

        virtual void            Serialize           (Json&  p_serialize)    const override;

    // ==============================[Public Local Methods]============================== //

        void                    AddAdditiveOffset       (Transform const&   p_newAdditiveOffset,
                                                         float const        p_newAdditiveFOVOffset);

        void                    ClearAdditiveOffset     ();

        FORCEINLINE float       GetAdditiveFOVOffset    ()  const;

        FORCEINLINE Transform   GetAdditiveOffset       ()  const;

    // ==============================[Public Local Operators]============================== //

        CameraComponent&    operator=   (CameraComponent const& p_copy) = default;

        CameraComponent&    operator=   (CameraComponent&&      p_move) = default;

    protected:

    // ==============================[Protected Local Properties]============================== //

        /** ... */
        bool        m_useAdditiveOffset;

        /** ... */
        Transform   m_additiveOffset;

        /** ... */
        float       m_additiveFOVOffset;

}; // !class CameraComponent

#include "CameraComponent.inl"
#include "CameraComponent.generated.hpp"

#endif // !__CAMERA_COMPONENT_HPP__