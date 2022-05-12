#ifndef __SPHERE_COMPONENT_HPP__
#define __SPHERE_COMPONENT_HPP__

#include "PrimitiveComponent.hpp"

class ENGINE_API SphereComponent : public PrimitiveComponent
{
    REFLECT(SphereComponent)

    typedef PrimitiveComponent Super;

    public:
    
    // ==============================[Public Constructor + Destructor]============================== //

        SphereComponent     ();

        SphereComponent     (SphereComponent const& p_copy) = default;

        SphereComponent     (SphereComponent&&      p_move) = default;

        ~SphereComponent    ()                              = default;

    // ==============================[Virtual Public Local Methods]============================== //

//      virtual BoxSphereBound  ComputeBounds           (Transform const&   p_localToWorld)         const override;
//
//      virtual void            ComputeBoundingCylinder (float&             p_cylinderRadius,
//                                                       float&             p_cylinderHalfHeight)   const override;

        virtual CollisionShape  GetCollisionShape       ()                                          const override;

        virtual EGeometryType   GetGeometryType         ()                                          const override;

        virtual bool            IsZeroExtent            ()                                          const override;

//      virtual void            UpdateBodySetup         ()                                          override;


    // ==============================[Public Local Methods]============================== //

        FORCEINLINE float   GetScaledSphereRadius   ()                                          const;

        FORCEINLINE float   GetShapeScale           ()                                          const;

        FORCEINLINE float   GetUnscaledSphereRadius ()                                          const;

        void                SetSphereRadius         (float const    p_newSphereRadius,
                                                     bool const     p_updateOverlaps = true);

        FORCEINLINE void    InitSphereRadius        (float const    p_sphereRadius);

    // ==============================[Public Local Operators]============================== //

        SphereComponent&    operator=   (SphereComponent const& p_copy) = default;

    protected:

    // ==============================[Protected Local Properties]============================== //

        /** The radius of sphere */
        PROPERTY()
        float   m_sphereRadius;

}; // !class SphereComponent

#include "SphereComponent.inl"
#include "SphereComponent.generated.hpp"

#endif // !__SPHERE_COMPONENT_HPP__
