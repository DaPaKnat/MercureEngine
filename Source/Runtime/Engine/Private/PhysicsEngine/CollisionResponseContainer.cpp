#include "PCH.hpp"

#include "PhysicsEngine/CollisionResponseContainer.hpp"

// ==============================[Public Static Properties]============================== //

CollisionResponseContainer  CollisionResponseContainer::DefaultResponseContainer    {ECollisionResponse::Ignore};

// ==============================[Public Constructor + Destructor]============================== //

/**
 * Default Constructor.
 */
CollisionResponseContainer::CollisionResponseContainer  ()
{
    *this = CollisionResponseContainer::DefaultResponseContainer;
}

/**
 * Constructor which initialize all channel with the same response.
 *
 * @param p_defaulfResponse The response to set at all channels.
 */
CollisionResponseContainer::CollisionResponseContainer  (ECollisionResponse const           p_defaulfResponse)
{
    SetAllChannels(p_defaulfResponse);
}

/**
 * Copy Constructor.
 */
CollisionResponseContainer::CollisionResponseContainer  (CollisionResponseContainer const&  p_copy)
:   m_worldStatic           {p_copy.m_worldStatic},
    m_worldDynamic          {p_copy.m_worldDynamic},
    m_gameTraceChannel1     {p_copy.m_gameTraceChannel1},
	m_gameTraceChannel2     {p_copy.m_gameTraceChannel2},
	m_gameTraceChannel3     {p_copy.m_gameTraceChannel3},
	m_gameTraceChannel4     {p_copy.m_gameTraceChannel4},
	m_gameTraceChannel5     {p_copy.m_gameTraceChannel5},
	m_gameTraceChannel6     {p_copy.m_gameTraceChannel6},
	m_gameTraceChannel7     {p_copy.m_gameTraceChannel7},
	m_gameTraceChannel8     {p_copy.m_gameTraceChannel8},
	m_gameTraceChannel9     {p_copy.m_gameTraceChannel9},
	m_gameTraceChannel10    {p_copy.m_gameTraceChannel10},
	m_gameTraceChannel11    {p_copy.m_gameTraceChannel11},
	m_gameTraceChannel12    {p_copy.m_gameTraceChannel12},
	m_gameTraceChannel13    {p_copy.m_gameTraceChannel13},
	m_gameTraceChannel14    {p_copy.m_gameTraceChannel14},
	m_gameTraceChannel15    {p_copy.m_gameTraceChannel15},
	m_gameTraceChannel16    {p_copy.m_gameTraceChannel16},
	m_gameTraceChannel17    {p_copy.m_gameTraceChannel17},
    m_gameTraceChannel18    {p_copy.m_gameTraceChannel18},
    m_gameTraceChannel19    {p_copy.m_gameTraceChannel19},
    m_gameTraceChannel20    {p_copy.m_gameTraceChannel20},
    m_gameTraceChannel21    {p_copy.m_gameTraceChannel21},
    m_gameTraceChannel22    {p_copy.m_gameTraceChannel22},
    m_gameTraceChannel23    {p_copy.m_gameTraceChannel23},
    m_gameTraceChannel24    {p_copy.m_gameTraceChannel24},
    m_gameTraceChannel25    {p_copy.m_gameTraceChannel25},
    m_gameTraceChannel26    {p_copy.m_gameTraceChannel26},
    m_gameTraceChannel27    {p_copy.m_gameTraceChannel27},
    m_gameTraceChannel28    {p_copy.m_gameTraceChannel28},
    m_gameTraceChannel29    {p_copy.m_gameTraceChannel29},
    m_gameTraceChannel30    {p_copy.m_gameTraceChannel30} 
{}

/**
 * Move Constructor.
 */
CollisionResponseContainer::CollisionResponseContainer  (CollisionResponseContainer&&       p_move)
:   m_worldStatic           {std::move(p_move.m_worldStatic)},
    m_worldDynamic          {std::move(p_move.m_worldDynamic)},
    m_gameTraceChannel1     {std::move(p_move.m_gameTraceChannel1)},
	m_gameTraceChannel2     {std::move(p_move.m_gameTraceChannel2)},
	m_gameTraceChannel3     {std::move(p_move.m_gameTraceChannel3)},
	m_gameTraceChannel4     {std::move(p_move.m_gameTraceChannel4)},
	m_gameTraceChannel5     {std::move(p_move.m_gameTraceChannel5)},
	m_gameTraceChannel6     {std::move(p_move.m_gameTraceChannel6)},
	m_gameTraceChannel7     {std::move(p_move.m_gameTraceChannel7)},
	m_gameTraceChannel8     {std::move(p_move.m_gameTraceChannel8)},
	m_gameTraceChannel9     {std::move(p_move.m_gameTraceChannel9)},
	m_gameTraceChannel10    {std::move(p_move.m_gameTraceChannel10)},
	m_gameTraceChannel11    {std::move(p_move.m_gameTraceChannel11)},
	m_gameTraceChannel12    {std::move(p_move.m_gameTraceChannel12)},
	m_gameTraceChannel13    {std::move(p_move.m_gameTraceChannel13)},
	m_gameTraceChannel14    {std::move(p_move.m_gameTraceChannel14)},
	m_gameTraceChannel15    {std::move(p_move.m_gameTraceChannel15)},
	m_gameTraceChannel16    {std::move(p_move.m_gameTraceChannel16)},
	m_gameTraceChannel17    {std::move(p_move.m_gameTraceChannel17)},
    m_gameTraceChannel18    {std::move(p_move.m_gameTraceChannel18)},
    m_gameTraceChannel19    {std::move(p_move.m_gameTraceChannel19)},
    m_gameTraceChannel20    {std::move(p_move.m_gameTraceChannel20)},
    m_gameTraceChannel21    {std::move(p_move.m_gameTraceChannel21)},
    m_gameTraceChannel22    {std::move(p_move.m_gameTraceChannel22)},
    m_gameTraceChannel23    {std::move(p_move.m_gameTraceChannel23)},
    m_gameTraceChannel24    {std::move(p_move.m_gameTraceChannel24)},
    m_gameTraceChannel25    {std::move(p_move.m_gameTraceChannel25)},
    m_gameTraceChannel26    {std::move(p_move.m_gameTraceChannel26)},
    m_gameTraceChannel27    {std::move(p_move.m_gameTraceChannel27)},
    m_gameTraceChannel28    {std::move(p_move.m_gameTraceChannel28)},
    m_gameTraceChannel29    {std::move(p_move.m_gameTraceChannel29)},
    m_gameTraceChannel30    {std::move(p_move.m_gameTraceChannel30)} 
{}

// ==============================[Public Local Methods]==============================//

/**
 * Sets the response of a particular channel.
 *
 * @param p_channel     The channel where wh set the response.
 * @param p_newResponse
 */
void    CollisionResponseContainer::SetResponse     (ECollisionChannel const    p_channel,
                                                     ECollisionResponse const   p_newResponse)
{
    m_enumArray[static_cast<uint8>(p_channel)] = static_cast<uint8>(p_newResponse);
}

/**
 * Replaces the channels matching the old response with new response.
 *
 * @param p_oldResponse The old responses to replace.
 * @param p_newResponse The new response to apply.
 */
void    CollisionResponseContainer::ReplaceChannels (ECollisionResponse const   p_oldResponse,
                                                     ECollisionResponse const   p_newResponse)
{
    for (int32 index {0} ; index < 32 ; index++)
    {
        if (m_enumArray[index] == static_cast<uint8>(p_oldResponse))
        {
            m_enumArray[index] = static_cast<uint8>(p_newResponse);
        }

    }   
}

/**
 * Set alls channels to the specified response.
 *
 * @param p_newResponse The new response to apply on all channels.
 */
void    CollisionResponseContainer::SetAllChannels  (ECollisionResponse const   p_newResponse)
{
    for (int32 index {0} ; index < 32 ; index++)
    {
        m_enumArray[index] = static_cast<uint8>(p_newResponse);
    }
}

// ==============================[Public Local Operators]==============================//

/**
 * Checks if the both collision response container are equals.
 *
 * @return True if they are equal, otherwise false.
 */
bool                        CollisionResponseContainer::operator==  (CollisionResponseContainer const&  p_other)    const
{
    return  m_worldStatic           == p_other.m_worldStatic        &&
            m_worldDynamic          == p_other.m_worldDynamic       &&
            m_gameTraceChannel1     == p_other.m_gameTraceChannel1  &&
	        m_gameTraceChannel2     == p_other.m_gameTraceChannel2  &&
	        m_gameTraceChannel3     == p_other.m_gameTraceChannel3  &&
	        m_gameTraceChannel4     == p_other.m_gameTraceChannel4  &&
	        m_gameTraceChannel5     == p_other.m_gameTraceChannel5  &&
	        m_gameTraceChannel6     == p_other.m_gameTraceChannel6  &&
	        m_gameTraceChannel7     == p_other.m_gameTraceChannel7  &&
	        m_gameTraceChannel8     == p_other.m_gameTraceChannel8  &&
	        m_gameTraceChannel9     == p_other.m_gameTraceChannel9  &&
	        m_gameTraceChannel10    == p_other.m_gameTraceChannel10 &&
	        m_gameTraceChannel11    == p_other.m_gameTraceChannel11 &&
	        m_gameTraceChannel12    == p_other.m_gameTraceChannel12 &&
	        m_gameTraceChannel13    == p_other.m_gameTraceChannel13 &&
	        m_gameTraceChannel14    == p_other.m_gameTraceChannel14 &&
	        m_gameTraceChannel15    == p_other.m_gameTraceChannel15 &&
	        m_gameTraceChannel16    == p_other.m_gameTraceChannel16 &&
	        m_gameTraceChannel17    == p_other.m_gameTraceChannel17 &&
            m_gameTraceChannel18    == p_other.m_gameTraceChannel18 &&
            m_gameTraceChannel19    == p_other.m_gameTraceChannel19 &&
            m_gameTraceChannel20    == p_other.m_gameTraceChannel20 &&
            m_gameTraceChannel21    == p_other.m_gameTraceChannel21 &&
            m_gameTraceChannel22    == p_other.m_gameTraceChannel22 &&
            m_gameTraceChannel23    == p_other.m_gameTraceChannel23 &&
            m_gameTraceChannel24    == p_other.m_gameTraceChannel24 &&
            m_gameTraceChannel25    == p_other.m_gameTraceChannel25 &&
            m_gameTraceChannel26    == p_other.m_gameTraceChannel26 &&
            m_gameTraceChannel27    == p_other.m_gameTraceChannel27 &&
            m_gameTraceChannel28    == p_other.m_gameTraceChannel28 &&
            m_gameTraceChannel29    == p_other.m_gameTraceChannel29 &&
            m_gameTraceChannel30    == p_other.m_gameTraceChannel30; 
}

/**
* Checks if the both collision response container are different.
*
* @return True if they are different, otherwise false.
*/
bool                        CollisionResponseContainer::operator!=  (CollisionResponseContainer const&  p_other)    const
{
    return !operator==(p_other);
}

/**
 * Copy the data of the parameter given into this one.
 *
 * @parma p_copy    The collision response container to copy on this one.
 *
 * @return The reference of this after copy.
 */
CollisionResponseContainer& CollisionResponseContainer::operator=   (CollisionResponseContainer const&  p_copy)
{
    m_worldStatic           = p_copy.m_worldStatic;
    m_worldDynamic          = p_copy.m_worldDynamic;
    m_gameTraceChannel1     = p_copy.m_gameTraceChannel1;
	m_gameTraceChannel2     = p_copy.m_gameTraceChannel2;
	m_gameTraceChannel3     = p_copy.m_gameTraceChannel3;
	m_gameTraceChannel4     = p_copy.m_gameTraceChannel4;
	m_gameTraceChannel5     = p_copy.m_gameTraceChannel5;
	m_gameTraceChannel6     = p_copy.m_gameTraceChannel6;
	m_gameTraceChannel7     = p_copy.m_gameTraceChannel7;
	m_gameTraceChannel8     = p_copy.m_gameTraceChannel8;
	m_gameTraceChannel9     = p_copy.m_gameTraceChannel9;
	m_gameTraceChannel10    = p_copy.m_gameTraceChannel10;
	m_gameTraceChannel11    = p_copy.m_gameTraceChannel11;
	m_gameTraceChannel12    = p_copy.m_gameTraceChannel12;
	m_gameTraceChannel13    = p_copy.m_gameTraceChannel13;
	m_gameTraceChannel14    = p_copy.m_gameTraceChannel14;
	m_gameTraceChannel15    = p_copy.m_gameTraceChannel15;
	m_gameTraceChannel16    = p_copy.m_gameTraceChannel16;
	m_gameTraceChannel17    = p_copy.m_gameTraceChannel17;
    m_gameTraceChannel18    = p_copy.m_gameTraceChannel18;
    m_gameTraceChannel19    = p_copy.m_gameTraceChannel19;
    m_gameTraceChannel20    = p_copy.m_gameTraceChannel20;
    m_gameTraceChannel21    = p_copy.m_gameTraceChannel21;
    m_gameTraceChannel22    = p_copy.m_gameTraceChannel22;
    m_gameTraceChannel23    = p_copy.m_gameTraceChannel23;
    m_gameTraceChannel24    = p_copy.m_gameTraceChannel24;
    m_gameTraceChannel25    = p_copy.m_gameTraceChannel25;
    m_gameTraceChannel26    = p_copy.m_gameTraceChannel26;
    m_gameTraceChannel27    = p_copy.m_gameTraceChannel27;
    m_gameTraceChannel28    = p_copy.m_gameTraceChannel28;
    m_gameTraceChannel29    = p_copy.m_gameTraceChannel29;
    m_gameTraceChannel30    = p_copy.m_gameTraceChannel30;

    return *this;
}

/**
 * Move the data of the parameter given into this one.
 *
 * @parma p_move    The collision response container to move on this one.
 *
 * @return The reference of this after move.
 */
CollisionResponseContainer& CollisionResponseContainer::operator=   (CollisionResponseContainer&&       p_copy)
{
    m_worldStatic           = std::move(p_copy.m_worldStatic);
    m_worldDynamic          = std::move(p_copy.m_worldDynamic);
    m_gameTraceChannel1     = std::move(p_copy.m_gameTraceChannel1);
	m_gameTraceChannel2     = std::move(p_copy.m_gameTraceChannel2);
	m_gameTraceChannel3     = std::move(p_copy.m_gameTraceChannel3);
	m_gameTraceChannel4     = std::move(p_copy.m_gameTraceChannel4);
	m_gameTraceChannel5     = std::move(p_copy.m_gameTraceChannel5);
	m_gameTraceChannel6     = std::move(p_copy.m_gameTraceChannel6);
	m_gameTraceChannel7     = std::move(p_copy.m_gameTraceChannel7);
	m_gameTraceChannel8     = std::move(p_copy.m_gameTraceChannel8);
	m_gameTraceChannel9     = std::move(p_copy.m_gameTraceChannel9);
	m_gameTraceChannel10    = std::move(p_copy.m_gameTraceChannel10);
	m_gameTraceChannel11    = std::move(p_copy.m_gameTraceChannel11);
	m_gameTraceChannel12    = std::move(p_copy.m_gameTraceChannel12);
	m_gameTraceChannel13    = std::move(p_copy.m_gameTraceChannel13);
	m_gameTraceChannel14    = std::move(p_copy.m_gameTraceChannel14);
	m_gameTraceChannel15    = std::move(p_copy.m_gameTraceChannel15);
	m_gameTraceChannel16    = std::move(p_copy.m_gameTraceChannel16);
	m_gameTraceChannel17    = std::move(p_copy.m_gameTraceChannel17);
    m_gameTraceChannel18    = std::move(p_copy.m_gameTraceChannel18);
    m_gameTraceChannel19    = std::move(p_copy.m_gameTraceChannel19);
    m_gameTraceChannel20    = std::move(p_copy.m_gameTraceChannel20);
    m_gameTraceChannel21    = std::move(p_copy.m_gameTraceChannel21);
    m_gameTraceChannel22    = std::move(p_copy.m_gameTraceChannel22);
    m_gameTraceChannel23    = std::move(p_copy.m_gameTraceChannel23);
    m_gameTraceChannel24    = std::move(p_copy.m_gameTraceChannel24);
    m_gameTraceChannel25    = std::move(p_copy.m_gameTraceChannel25);
    m_gameTraceChannel26    = std::move(p_copy.m_gameTraceChannel26);
    m_gameTraceChannel27    = std::move(p_copy.m_gameTraceChannel27);
    m_gameTraceChannel28    = std::move(p_copy.m_gameTraceChannel28);
    m_gameTraceChannel29    = std::move(p_copy.m_gameTraceChannel29);
    m_gameTraceChannel30    = std::move(p_copy.m_gameTraceChannel30);

    return *this;
}