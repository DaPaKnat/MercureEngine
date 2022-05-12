#include "PCH.hpp"

#include "EngineTypes/DetachmentTransformRules.hpp"

// ==============================[Public Static Properties] ============================== //

DetachmentTransformRules    DetachmentTransformRules::KeepRelative  {EDetachmentRule::KeepRelative};
DetachmentTransformRules    DetachmentTransformRules::KeepWorld     {EDetachmentRule::KeepWorld};

// ==============================[Constructor + Destructor] ============================== //

/** Default constructor, all components are set as KeepWorld*/
DetachmentTransformRules::DetachmentTransformRules  ()
:   m_locationRule  {EDetachmentRule::KeepWorld},
    m_rotationRule  {EDetachmentRule::KeepWorld},
    m_scaleRule     {EDetachmentRule::KeepWorld}
{}

/** 
 * Constructor using initial all component rules with a single rule value.
 *
 * @param p_rule    The rule to sett all components to
 */
DetachmentTransformRules::DetachmentTransformRules  (EDetachmentRule const              p_rule)
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
DetachmentTransformRules::DetachmentTransformRules  (EDetachmentRule const              p_locationRule,
                                                     EDetachmentRule const              p_rotationRule,
                                                     EDetachmentRule const              p_scaleRule)
:   m_locationRule  {p_locationRule},
    m_rotationRule  {p_rotationRule},
    m_scaleRule     {p_scaleRule}
{}

/**
 * Copy constructor using initial DetachmentTransformRules for each component.
 *
 * @param p_copy    The initial DetachmentTransformRules  to copy.
 */
DetachmentTransformRules::DetachmentTransformRules  (DetachmentTransformRules const&    p_copy)
:   m_locationRule  {p_copy.m_locationRule},
    m_rotationRule  {p_copy.m_rotationRule},
    m_scaleRule     {p_copy.m_scaleRule}
{}

/**
 * Move constructor using initial DetachmentTransformRules for each component.
 *
 * @param p_copy    The initial DetachmentTransformRules to Move.
 */
DetachmentTransformRules::DetachmentTransformRules  (DetachmentTransformRules&&         p_move)
:   m_locationRule  {std::move(p_move.m_locationRule)},
    m_rotationRule  {std::move(p_move.m_rotationRule)},
    m_scaleRule     {std::move(p_move.m_scaleRule)}
{}

// ==============================[Public Local Operators] ============================== //

/**
 * Checks against another DetachmentTransformRules for equality.
 *
 * @param p_other   The DetachmentTransformRules to check against.
 *
 * @return True if they this and p_other are equal, otherwise false.
 */
bool                        DetachmentTransformRules::operator==    (DetachmentTransformRules const&    p_other) const
{
    return  m_locationRule == p_other.m_locationRule &&
            m_rotationRule == p_other.m_rotationRule &&
            m_scaleRule == p_other.m_scaleRule;
}

/**
 * Checks against another DetachmentTransformRules for inequality.
 *
 * @param p_other   The DetachmentTransformRules to check against.
 *
 * @return True if they this and p_other are different, otherwise false.
 */
bool                        DetachmentTransformRules::operator!=    (DetachmentTransformRules const&    p_other) const
{
    return  m_locationRule != p_other.m_locationRule ||
            m_rotationRule != p_other.m_rotationRule ||
            m_scaleRule != p_other.m_scaleRule;
}

/**
 * Copy an another DetachmentTransformRules into this one.
 *
 * @param p_copy    The DetachmentTransformRules to copy.
 *
 * @return The reference to this DetachmentTransformRules after copy.
 */
DetachmentTransformRules&   DetachmentTransformRules::operator=     (DetachmentTransformRules const&    p_copy)
{
    m_locationRule  = p_copy.m_locationRule;
    m_rotationRule  = p_copy.m_rotationRule;
    m_scaleRule     = p_copy.m_scaleRule;

    return *this;
}

/**
 * Move an another DetachmentTransformRules into this one.
 *
 * @param p_copy    The DetachmentTransformRules to copy.
 *
 * @return The reference to this DetachmentTransformRules after Move.
 */
DetachmentTransformRules&   DetachmentTransformRules::operator=     (DetachmentTransformRules&&         p_move)
{
    m_locationRule  = p_move.m_locationRule;
    m_rotationRule  = p_move.m_rotationRule;
    m_scaleRule     = p_move.m_scaleRule;

    return *this;
}