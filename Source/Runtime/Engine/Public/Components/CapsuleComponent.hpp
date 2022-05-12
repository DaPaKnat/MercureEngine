#ifndef __CAPSULE_COMPONENT_HPP__
#define __CAPSULE_COMPONENT_HPP__

#include "PrimitiveComponent.hpp"

class ENGINE_API CapsuleComponent : public PrimitiveComponent
{
	REFLECT(CapsuleComponent)

    typedef PrimitiveComponent Super;

    public:

    // ====================CapsuleComponent==========[Public Constructor + Destructor]============================== //

        CapsuleComponent    ();

        CapsuleComponent    (CapsuleComponent const& p_copy)    = default;

        CapsuleComponent    (CapsuleComponent&&      p_move)    = default;

        ~CapsuleComponent   ()                                  = default;

    // ==============================[Virtual Public Local Methods]============================== //
       
//      virtual void            ComputeBoundingCylinder (float&             p_cylinderRadius,
//                                                       float&             p_cylinderHalfHeight)   const override;

//      virtual BoxSphereBound  ComputeBounds           (Transform const&   p_localToWorld)         const override;

        virtual CollisionShape  GetCollisionShape       ()                                          const override;

        virtual EGeometryType   GetGeometryType         ()                                          const override;

        virtual bool            IsZeroExtent            ()                                          const override;

//      virtual void            UpdateBodySetup         ()                                          override;

    // ==============================[Public Local Methods]============================== //

        FORCEINLINE void    SetCapsuleHalfHeight                            (float const    p_newHalfHeight,
                                                                             bool const     p_updateOverlaps = true);

        void                SetCapsuleRadius                                (float const    p_newRadius,
                                                                             bool const     p_updateOverlaps = true);

        void                SetCapsuleSize                                  (float const    p_newRadius,
                                                                             float const    p_newHalfHeight,
                                                                             bool const     p_updateOverlaps = true);

        FORCEINLINE float   GetScaledCapsuleHalfHeight                      ()                                                      const;

        FORCEINLINE float   GetScaledCapsuleHalfHeightWithoutHemisphere     ()                                                      const;

        FORCEINLINE float   GetScaledCapsuleRadius                          ()                                                      const;

        FORCEINLINE void    GetScaledCapsuleSize                            (float&         p_outScaledRadius,
                                                                             float&         p_outScaledHalfHeight)                  const;

        FORCEINLINE void    GetScaledCapsuleSizeWithoutHemisphere           (float&         p_outScaledRadius,
                                                                             float&         p_outScaledHalfHeightWithoutHemisphere) const;

        FORCEINLINE float   GetShapeScale                                   ()                                                      const;

        FORCEINLINE float   GetUnscaledCapsuleHalfHeight                    ()                                                      const;

        FORCEINLINE float   GetUnscaledCapsuleHalfHeightWithoutHemisphere   ()                                                      const;

        FORCEINLINE float   GetUnscaledCapsuleRadius                        ()                                                      const;

        FORCEINLINE void    GetUnscaledCapsuleSize                          (float&         p_outScaledRadius,
                                                                             float&         p_outScaledHalfHeight)                  const;

        FORCEINLINE void    GetUnscaledCapsuleSizeWithoutHemisphere         (float&         p_outScaledRadius,
                                                                             float&         p_outScaledHalfHeightWithoutHemisphere) const;


        FORCEINLINE void    InitCapsuleSize                                 (float const    p_radius,
                                                                             float const    p_halfHeight);

        CapsuleComponent&   operator=   (CapsuleComponent const&) = default;
    protected:
    
    // ==============================[Protected Local Properties]============================== //

        /**
         * Half-height, from center of capsule to the end of top or bottom hemisphere.
         * This cannot be less than Capsule Radius.
         */
		PROPERTY()
        float   m_capsuleHalfHeight;

        /**
         * Radis of cap hemispheres and center cylinder.
         * This cannot be more than Capsule Half Height.
         */
		PROPERTY()
        float   m_capsuleRadius;
    
}; // !class CapsuleComponent

#include "CapsuleComponent.inl"
#include "CapsuleComponent.generated.hpp"

#endif // !__CAPSULE_COMPONENT_HPP__