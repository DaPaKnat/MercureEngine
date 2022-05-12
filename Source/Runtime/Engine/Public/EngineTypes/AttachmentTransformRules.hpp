#ifndef __ATTACHMENT_TRANSFORM_RULES_HPP__
#define __ATTACHMENT_TRANSFORM_RULES_HPP__

#include "CoreMinimal.hpp"

// ==============================[Enumerators Declaration]============================== //

enum class EAttachmentRule
{
    /** Keeps current relatvie transform as the relative transform to the new parent */
    KeepRelative,

    /** Calculates the relative transform such that the attached component maintains the same world transform*/
    KeepWorld,

    /** Snaps transform to the attach point */
    SnapToTarget

}; // !enum class EAttachmentRule

// ===================================================================================== //

struct ENGINE_API AttachmentTransformRules
{
    // ==============================[Public Static Properties] ============================== //

        /** Preset for all EDetachRule components are set as KeepRelative */
        static  AttachmentTransformRules    KeepRelative;

        /** Preset for all EDetachRule components are set as KeepWorl */
        static  AttachmentTransformRules    KeepWorld;

        /** Preset for all EDetachRule components are set as SnapToTarget except the scale which is set as KeepWorld */
        static  AttachmentTransformRules    SnapToTargetNotIncludingScale;

        /** Preset for all EDetachRule components are set as SnapToTarget */
        static  AttachmentTransformRules    SnapToTargetIncludingScale;

    // ==============================[Public Local Properties] ============================== //

        /** The rule to apply to location when attaching */
        EAttachmentRule m_locationRule;

        /** The rule to apply to rotation when attaching */
        EAttachmentRule m_rotationRule;

        /** The rule to apply to scale when attaching */
        EAttachmentRule m_scaleRule;


    // ==============================[Constructor + Destructor] ============================== //

        AttachmentTransformRules    ();

        AttachmentTransformRules    (EAttachmentRule const              p_rule);

        AttachmentTransformRules    (EAttachmentRule const              p_locationRule,
                                     EAttachmentRule const              p_rotationRule,
                                     EAttachmentRule const              p_scaleRule);
        
        AttachmentTransformRules    (AttachmentTransformRules const&    p_copy);

        AttachmentTransformRules    (AttachmentTransformRules&&         p_move);

        ~AttachmentTransformRules   () = default;

    // ==============================[Public Local Operators] ============================== //
    
        bool                        operator==  (AttachmentTransformRules const&    p_other) const;
        bool                        operator!=  (AttachmentTransformRules const&    p_other) const;

        AttachmentTransformRules&   operator=   (AttachmentTransformRules const&    p_copy);
        AttachmentTransformRules&   operator=   (AttachmentTransformRules&&         p_move);

}; // !struct AttachmentTransformRules

#endif // !__ATTACHMENT_TRANSFORM_RULES_HPP__
