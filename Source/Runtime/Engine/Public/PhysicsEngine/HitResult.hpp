#ifndef __HIT_RESULT_HPP__
#define __HIT_RESULT_HPP__

#include "CoreMinimal.hpp"

// ==============================[Forward Declaration]============================== //

    class Entity;
    class PrimitiveComponent;

// ================================================================================= //

/**
 * Structure containing information about one hit of trace.
 */
struct ENGINE_API HitResult
{
    // ==============================[Public Local Properties]============================== //

        /** The actor that was hit by the trace. */
        Entity*              m_entity;

        /** 
         * The distance from the TraceStart to the impact Location in world space. 
         * This value is 0 if there was an inital overlap 
         */
        float               m_distance;

        /**
        * Location in world space of the actual contact of the trace shape (box, sphere, ray, etc) with the impacted object.
        * Example: for a sphere trace test, this is the point where the surface of the sphere touches the other object.
        * Note: In the case of initial overlap (bStartPenetrating=true), ImpactPoint will be the same as Location because there is no meaningful single impact point to report.
        */
        Vector3             m_impactLocation;

	    /**
	     * Normal of the hit in world space, for the object that was hit by the sweep, if any.
	     * For example if a box hits a flat plane, this is a normalized vector pointing out from the plane.
	     * In the case of impact with a corner or edge of a surface, usually the "most opposing" normal (opposed to the query direction) is chosen.
	     */
        Vector3             m_impactNormal;

        /**
	     * 'Time' of impact along trace direction (ranging from 0.0 to 1.0) if there is a hit, indicating time between TraceStart and TraceEnd.
	     * For swept movement (but not queries) this may be pulled back slightly from the actual time of impact, to prevent precision problems with adjacent geometry.
	     */
        bool                m_isBlockingHit;
        
	    /**
	     * The location in world space where the moving shape would end up against the impacted object, if there is a hit. Equal to the point of impact for line tests.
	     * Example: for a sphere trace test, this is the point where the center of the sphere would be located when it touched the other object.
	     * For swept movement (but not queries) this may not equal the final location of the shape since hits are pulled back slightly to prevent precision issues from overlapping another surface.
	     */
        Vector3             m_location;

	    /**
	     * Normal of the hit in world space, for the object that was swept. Equal to ImpactNormal for line tests.
	     * This is computed for capsules and spheres, otherwise it will be the same as ImpactNormal.
	     * Example: for a sphere trace test, this is a normalized vector pointing in towards the center of the sphere at the point of impact.
	     */
        Vector3             m_normal;

	    /**
	     * If this test started in penetration (bStartPenetrating is true) and a depenetration vector can be computed,
	     * this value is the distance along Normal that will result in moving out of penetration.
	     * If the distance cannot be computed, this distance will be zero.
	     */
        float               m_penetrationDepth;

        /** The primivitve component that was hit by the trace */
        PrimitiveComponent* m_primitiveComponent;

        /** The end location of the trace */
        Vector3             m_traceEnd;

        /** The start location of the trace */
        Vector3             m_traceStart;

        /**
	     * Whether the trace started in penetration, i.e. with an initial blocking overlap.
	     * In the case of penetration, if PenetrationDepth > 0.f, then it will represent the distance along the Normal vector that will result in
	     * minimal contact between the swept shape and the object that was hit. In this case, ImpactNormal will be the normal opposed to movement at that location
	     * (ie, Normal may not equal ImpactNormal). ImpactPoint will be the same as Location, since there is no single impact point to report.
	     */
        bool                m_startPenetrating;

        /** Value between 0 and 1. */
        float               m_hitTime;

        /** ... */
        // PhysicMaterial   m_physicMateria;

    // ==============================[Public Constructor + Destructor]============================== //

        HitResult   ();

        HitResult   (Vector3 const&     p_startTrace,
                     Vector3 const&     p_endTrace);

        HitResult   (HitResult const&   p_copy);

        HitResult   (HitResult&&        p_move);

        ~HitResult  ()                               = default;

    // ==============================[Public Local Methods]============================== //

        bool    IsValidBlockingHit  ()  const
        {
            return m_isBlockingHit && !m_startPenetrating;
        }

    // ==============================[Public Local Operators]============================== //

        HitResult&  operator=   (HitResult const&   p_copy);
        HitResult&  operator=   (HitResult&&        p_move);

}; // !struct HitResult

#endif // !__HIT_RESULT_HPP__