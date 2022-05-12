#ifndef __COLLISION_SHAPE_HPP__
#define __COLLISION_SHAPE_HPP__

#include "CoreMinimal.hpp"

// ==============================[Enums Declaration]============================== //

/** Types of Collision Shapes that are used by trace. */
enum ENGINE_API ECollisionShapeType
{
    Box,
    Capsule,
    Sphere,
    None

}; // !enum class ECollisionShape

// ================================================================================ //

/** Collision Shape that can represent Box, Capsule, Line or Sphere. */
struct ENGINE_API CollisionShape
{
    // ==============================[Public Static Methods]============================== //
    
        static CollisionShape   MakeBox     (Vector3 const& p_halfExtent);

        static CollisionShape   MakeBox     (float const    p_halfExtentX,
                                             float const    p_halfExtentY,
                                             float const    p_halfExtentZ);

        static CollisionShape   MakeCapsule (float const    p_radius,
                                             float const    p_halfHeight);

        static CollisionShape   MakeCapsule (Vector3 const& p_extent);

        static CollisionShape   MakeSphere  (float const    p_radius);

    // ==============================[Public Local Properties]============================== //

        /** Type of the collision Shape */
        ECollisionShapeType m_shapeType;
        
        /** Union that support up 3 float in order to be able to reprents all kind of CollisionShape. */
        union
        {
            /** Structure representing a box Shape */
            struct 
            {
                /** Half Extent of the box. */
                Vector3 m_halfExtent;
            
            } Box; // !struct Box

            /** Structure representing a capsule Shape */
            struct 
            {
                /**
                 * Half-height, from center of capsule to the end of top or bottom hemisphere.
                 * This cannot be less than Capsule Radius.
                 */
                float m_halfHeight;

                /**
                 * Radis of cap hemispheres and center cylinder.
                 * This cannot be more than Capsule Half Height.
                 */
                float m_radius;

            } Capsule; // !struct Capsule

            /** Structure representing a sphere Shape */
            struct 
            {
                /** Radius of the sphere */
                float   m_radius;

            } Sphere; // !struct Sphere
        };

    // ==============================[Public Constructor + Destructor]============================== //

            CollisionShape  ();
            
            CollisionShape  (CollisionShape const&  p_copy);

            CollisionShape  (CollisionShape&&       p_move);

            ~CollisionShape ()                                  = default;


    // ==============================[Public Local Methods]============================== //

        FORCEINLINE Vector3 GetBoxHalfExtent                        ()                                      const;

        Vector3             GetExtent                               ()                                      const;

        FORCEINLINE float   GetCapsuleHalfHeightWithoutHemisphere   ()                                      const;

        FORCEINLINE float   GetCapsuleHalfHeight                    ()                                      const;    

        FORCEINLINE float   GetCapsuleRadius                        ()                                      const;

        FORCEINLINE float   GetSphereRadius                         ()                                      const;

        FORCEINLINE bool    IsBox                                   ()                                      const;
                       
        FORCEINLINE bool    IsCapsule                               ()                                      const;

        bool                IsNearlyZero()                                                                  const;

        FORCEINLINE bool    IsSphere                                ()                                      const;

        FORCEINLINE void    SetBox                                  (Vector3 const& p_halfExtent);

        FORCEINLINE void    SetBox                                  (float const    p_halfExtentX,
                                                                     float const    p_halfExtentY,
                                                                     float const    p_halfExtentZ);

        FORCEINLINE void    SetCapsule                              (float const    p_radius,
                                                                     float const    p_halfHeight);

        FORCEINLINE void    SetCapsule                              (Vector3 const& p_extent);

        FORCEINLINE void    SetSphere                               (float const    p_radius);

}; // !struct CollisionShape

#include "CollisionShape.inl"

#endif // !__COLLISION_SHAPE_HPP__