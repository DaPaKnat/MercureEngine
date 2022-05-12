#ifndef __DETACHMENT_TRANSFORM_RULES_HPP__
#define __DETACHMENT_TRANSFORM_RULES_HPP__

#include "CoreMinimal.hpp"

enum class EDetachmentRule
{
    /** Keeps current relatvie transform as the relative transform to the new parent */
    KeepRelative,

    /** Calculates the relative transform such that the attached component maintains the same world transform*/
    KeepWorld,

}; // !enum class EDetachRule

// ===================================================================================== //

struct ENGINE_API DetachmentTransformRules
{
    // ==============================[Public Static Properties] ============================== //

        /** Preset for all EDetachRule components are set as KeepRelative */
        static  DetachmentTransformRules    KeepRelative;

        /** Preset for all EDetachRule components are set as KeepWorl */
        static  DetachmentTransformRules    KeepWorld;

    // ==============================[Public Local Properties] ============================== //

        /** The rule to apply to location when attaching */
        EDetachmentRule m_locationRule;

        /** The rule to apply to rotation when attaching */
        EDetachmentRule m_rotationRule;

        /** The rule to apply to scale when attaching */
        EDetachmentRule m_scaleRule;


    // ==============================[Constructor + Destructor] ============================== //

        DetachmentTransformRules    ();

        DetachmentTransformRules    (EDetachmentRule const              p_rule);

        DetachmentTransformRules    (EDetachmentRule const              p_locationRule,
                                     EDetachmentRule const              p_rotationRule,
                                     EDetachmentRule const              p_scaleRule);
        
        DetachmentTransformRules    (DetachmentTransformRules const&    p_copy);

        DetachmentTransformRules    (DetachmentTransformRules&&         p_move);

        ~DetachmentTransformRules   () = default;

    // ==============================[Public Local Operators] ============================== //
    
        bool                        operator==  (DetachmentTransformRules const&    p_other) const;
        bool                        operator!=  (DetachmentTransformRules const&    p_other) const;

        DetachmentTransformRules&   operator=   (DetachmentTransformRules const&    p_copy);
        DetachmentTransformRules&   operator=   (DetachmentTransformRules&&         p_move);

}; // !struct DetachmentTransformRules

#endif  // !__DETACHMENT_TRANSFORM_RULES_HPP__