#include "PCH.hpp"

#include "EngineTypes/AttachmentTransformRules.hpp"

// ==============================[Public Static Properties] ============================== //

AttachmentTransformRules    AttachmentTransformRules::KeepRelative                  {EAttachmentRule::KeepRelative};
AttachmentTransformRules    AttachmentTransformRules::KeepWorld                     {EAttachmentRule::KeepWorld};
AttachmentTransformRules    AttachmentTransformRules::SnapToTargetNotIncludingScale {EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld};
AttachmentTransformRules    AttachmentTransformRules::SnapToTargetIncludingScale    {EAttachmentRule::SnapToTarget};

// ==============================[Constructor + Destructor] ============================== //

/** Default constructor, all components are set as KeepWorld*/
AttachmentTransformRules::AttachmentTransformRules  ()
:   m_locationRule  {EAttachmentRule::KeepWorld},
    m_rotationRule  {EAttachmentRule::KeepWorld},
    m_scaleRule     {EAttachmentRule::KeepWorld}
{}

/** 
 * Constructor using initial all component rules with a single rule value.
 *
 * @param p_rule    The rule to sett all components to
 */
AttachmentTransformRules::AttachmentTransformRules  (EAttachmentRule const              p_rule)
:   m_locationRule  {p_rule},
    m_rotationRule  {p_rule},
    m_scaleRule     {p_rule}
{}

/**
 * Constructor using initial rules value for each rule component.
 *
 * @param p_locationRule    The location rule.
 * @param p_rotationRule    The rotation rule.
 * @param p_scaleRule       The scale rule.
 */
AttachmentTransformRules::AttachmentTransformRules  (EAttachmentRule const              p_locationRule,
                                                     EAttachmentRule const              p_rotationRule,
                                                     EAttachmentRule const              p_scaleRule)
:   m_locationRule  {p_locationRule},
    m_rotationRule  {p_rotationRule},
    m_scaleRule     {p_scaleRule}
{}

/**
 * Copy constructor using initial AttachmentTransformRules for each component.
 *
 * @param p_copy    The initial AttachmentTransformRules  to copy.
 */
AttachmentTransformRules::AttachmentTransformRules  (AttachmentTransformRules const&    p_copy)
:   m_locationRule  {p_copy.m_locationRule},
    m_rotationRule  {p_copy.m_rotationRule},
    m_scaleRule     {p_copy.m_scaleRule}
{}

/**
 * Move constructor using initial AttachmentTransformRules for each component.
 *
 * @param p_copy    The initial AttachmentTransformRules to Move.
 */
AttachmentTransformRules::AttachmentTransformRules  (AttachmentTransformRules&&         p_move)
:   m_locationRule  {std::move(p_move.m_locationRule)},
    m_rotationRule  {std::move(p_move.m_rotationRule)},
    m_scaleRule     {std::move(p_move.m_scaleRule)}
{}

// ==============================[Public Local Operators] ============================== //

/**
 * Checks against another AttachmentTransformRules for equality.
 *
 * @param p_other   The AttachmentTransformRules to check against.
 *
 * @return True if they this and p_other are equal, otherwise false.
 */
bool                        AttachmentTransformRules::operator==    (AttachmentTransformRules const&    p_other) const
{
    return  m_locationRule == p_other.m_locationRule &&
            m_rotationRule == p_other.m_rotationRule &&
            m_scaleRule == p_other.m_scaleRule;
}

/**
 * Checks against another AttachmentTransformRules for inequality.
 *
 * @param p_other   The AttachmentTransformRules to check against.
 *
 * @return True if they this and p_other are different, otherwise false.
 */
bool                        AttachmentTransformRules::operator!=    (AttachmentTransformRules const&    p_other) const
{
    return  m_locationRule != p_other.m_locationRule ||
            m_rotationRule != p_other.m_rotationRule ||
            m_scaleRule != p_other.m_scaleRule;
}

/**
 * Copy an another AttachmentTransformRules into this one.
 *
 * @param p_copy    The AttachmentTransformRules to copy.
 *
 * @return The reference to this AttachmentTransformRules after copy.
 */
AttachmentTransformRules&   AttachmentTransformRules::operator=     (AttachmentTransformRules const&    p_copy)
{
    m_locationRule  = p_copy.m_locationRule;
    m_rotationRule  = p_copy.m_rotationRule;
    m_scaleRule     = p_copy.m_scaleRule;

    return *this;
}

/**
 * Move an another AttachmentTransformRules into this one.
 *
 * @param p_copy    The AttachmentTransformRules to copy.
 *
 * @return The reference to this AttachmentTransformRules after Move.
 */
AttachmentTransformRules&   AttachmentTransformRules::operator=     (AttachmentTransformRules&&         p_move)
{
    m_locationRule  = p_move.m_locationRule;
    m_rotationRule  = p_move.m_rotationRule;
    m_scaleRule     = p_move.m_scaleRule;

    return *this;
}