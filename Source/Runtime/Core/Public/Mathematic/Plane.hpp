#ifndef __PLANE_HPP__
#define __PLANE_HPP__

#include "Vector4.hpp"

/**
 * Respresentation of a plane in 3D Space.
 *
 * Cartesian Equation : Ax + By + Cz + d = 0.
 * Normal (A,B,C) & M(x,y,z) point belonging to the plane.
 */
struct ENGINE_API Plane
{
	REFLECT(Plane)

    //=========================[Public Static Methods]=========================//

        static  FORCEINLINE bool    IsNearlyEqual   (Plane const&   p_a,
                                                     Plane const&   p_b,
                                                     float const    p_tolerance = MATH_EPSILON_F);

    //=========================[Public Local Properties]=========================//

        /** Normal of the plane */
		PROPERTY()
        Vector3 m_normal;

        /** Short distance from the plane to the origin of the space (0,0,0) */
		PROPERTY()
        float   m_distance;

    //=========================[Constructor + Destructor]=========================//

        FORCEINLINE Plane   ();

        FORCEINLINE Plane   (float const    p_xNormal,
                             float const    p_yNormal,
                             float const    p_zNormal,
                             float const    p_distance);
        
        FORCEINLINE Plane   (Vector4 const& p_vector4);

        FORCEINLINE Plane   (Vector3 const& p_pointA,
                             Vector3 const& p_pointB,
                             Vector3 const& p_pointC);

        FORCEINLINE Plane   (Vector3 const& p_normal,
                             Vector3 const& p_point);

        FORCEINLINE Plane   (Vector3 const& p_normal,
                             float const    p_distance);

        FORCEINLINE Plane   (Plane const&   p_copy);

        FORCEINLINE Plane   (Plane&&        p_move);

                    ~Plane  () = default;

    //=========================[Public Local Methods]=========================//

        FORCEINLINE void    Flip                    ();

        FORCEINLINE Vector3 GetProjectedPoint       (Vector3 const& p_point)        const;
        
        FORCEINLINE Vector3 GetProjectedVector      (Vector3 const& p_vector)       const;

        FORCEINLINE float   GetPointSignedDistance  (Vector3 const& p_point)        const;

        FORCEINLINE float   GetPointDistance        (Vector3 const& p_point)        const;

        FORCEINLINE Plane   GetFlipped              ()                              const;

        FORCEINLINE Plane   GetTranslated           (Vector3 const& p_translation)  const;

        FORCEINLINE bool    IsSameSide              (Vector3 const& p_pointA,
                                                     Vector3 const& p_pointB)       const;

        FORCEINLINE void    Translate               (Vector3 const& p_translation);

    //=========================[Public Local Operators]=========================//

        FORCEINLINE bool    operator==  (Plane const&   p_plane)    const;
        FORCEINLINE bool    operator!=  (Plane const&   p_plane)    const;

        FORCEINLINE Plane&  operator=   (Plane const&   p_copy);
        FORCEINLINE Plane&  operator=   (Plane&&        p_move);

}; // !struct Plane

#include "Plane.inl"
#include "Plane.generated.hpp"

#endif // !__PLANE_HPP__
