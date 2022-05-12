#ifndef __COLOR_32_INL__
#define __COLOR_32_INL__

#include "Color.hpp"

/*==============================[Constructors + Destructor]==============================*/

/** Default constructor, all components are initialized to 0 except alpha = 255 */
FORCEINLINE Color32::Color32    ()
:   m_rgba  {255}
{}

/**
 * Constructor using initial values for each component.
 *
 * @param p_rgba    The RGBA color contained in an Integer.
 */
FORCEINLINE Color32::Color32    (uint32 const   p_rgb)
:   m_rgba  {p_rgb}
{}

/**
 * Constructor using initial values for each component.
 *
 * @param p_r   The red component. Must be in range [0,255].
 * @param p_g   The green component. Must be in range [0,255].
 * @param p_b   The blue component. Must be in range [0,255].
 * @param p_a   The alpha component. Must be in range [0,255].
 */
FORCEINLINE Color32::Color32    (uint8 const    p_r,
                                 uint8 const    p_g,
                                 uint8 const    p_b,
                                 uint8 const    p_a)
:   m_r {p_r},
    m_g {p_g},
    m_b {p_b},
    m_a {p_a}
{}

/**
 * Constructor which convert a Color to a Color32.
 *
 * @param p_color   The Color to convert to Color32.
 */
FORCEINLINE Color32::Color32    (Color const&   p_color)
:   m_r {(uint8)(Math::Clamp01(p_color.m_r) * 255)},
    m_g {(uint8)(Math::Clamp01(p_color.m_g) * 255)},
    m_b {(uint8)(Math::Clamp01(p_color.m_b) * 255)},
    m_a {(uint8)(Math::Clamp01(p_color.m_a) * 255)}
{}

/**
 * Copy constructor using initial Color32 for each component.
 *
 * @param p_copy    The initial Color32 to copy.
 */
FORCEINLINE Color32::Color32    (Color32 const& p_copy)
:   m_rgba  {p_copy.m_rgba}
{}

/**
 * Move constructor using initial Color32 for each component.
 *
 * @param p_move    The initial Color32 to p_move.
 */
FORCEINLINE Color32::Color32    (Color32&&      p_move)
:   m_rgba  {std::move(p_move.m_rgba)}
{}

/*==============================[Public Local Methods]==============================*/

/**
 * Gets a specific component of the color by index.
 *
 * @param p_index   The index of the component required.
 *
 * @return          The copy of the specified component.
 */
FORCEINLINE uint8   Color32::Component  (int32 const    p_index)    const
{
    if (p_index < 0 || 3 < p_index)
        throw std::out_of_range("p_index is out of Color32's index range : [0,3]");

    if      (p_index == 0)
        return m_r;
    else if (p_index == 1)
        return m_g;
    else if (p_index == 2)
        return m_b;
    else
        return m_a;
}

/**
 * Gets a specific component of the color by index.
 *
 * @param p_index   The index of the component required.
 *
 * @return          The reference to the specified component.
 */
FORCEINLINE uint8&  Color32::Component  (int32 const    p_index)
{
    if (p_index < 0 || 3 < p_index)
        throw std::out_of_range("p_index is out of Color32's index range : [0,3]");

    if      (p_index == 0)
        return m_r;
    else if (p_index == 1)
        return m_g;
    else if (p_index == 2)
        return m_b;
    else
        return m_a;
}

/**
 * Get the maximum value of the Color32's components.
 *
 * @return  The maximum value of the color's components.
 */
FORCEINLINE uint8   Color32::GetMax     ()                          const
{
    return Math::Max(m_r,Math::Max(m_g, Math::Max(m_b,m_a)));
}

/**
 * Get the maximum value of the Color32's RGB components.
 *
 * @return  The maximum value of the RGB color's components.
 */
FORCEINLINE uint8   Color32::GetMaxRGB  ()                          const
{
    return Math::Max3(m_r, m_g, m_b);
}

/**
 * Get the minimum value of the Color32's RGB components.
 *
 * @return  The minimum value of the RGB color's components.
 */
FORCEINLINE uint8   Color32::GetMin     ()                          const
{
    return Math::Min(m_r,Math::Min(m_g, Math::Min(m_b,m_a)));
}

/**
 * Get the minimum value of the Color32's components.
 *
 * @return  The minimum value of the color's components.
 */
FORCEINLINE uint8   Color32::GetMinRGB  ()                          const
{
    return Math::Min3(m_r, m_g, m_b);
}

/**
 * Creater Color with this color
 *
 * @return This color converted into a Color32.
 */
FORCEINLINE Color   Color32::ToColor    ()                          const
{
    return Color
    (
        m_r / 255.0f,
        m_g / 255.0f,
        m_b / 255.0f,
        m_a / 255.0f
    );
}

/*==============================[Public Local Operators]==============================*/

/**
 * Check against another color for equality.
 *
 * @param p_color   The color to check against.
 *
 * @return          True    if the colors are equal.
 * @return          False   if the colors are different.
 */
FORCEINLINE bool        Color32::operator== (Color32 const& p_color)    const
{
    return  m_r == p_color.m_r && 
            m_g == p_color.m_g && 
            m_b == p_color.m_b &&
            m_a == p_color.m_a;
}

/**
 * Check against another color for inequality.
 *
 * @param p_vector  The color to check against.
 *
 * @return          True    if the colors are different.
 * @return          False   if the colors are equal.
 */
FORCEINLINE bool        Color32::operator!= (Color32 const& p_color)    const
{
    return  m_r != p_color.m_r || 
            m_g != p_color.m_g || 
            m_b != p_color.m_b || 
            m_a != p_color.m_a;
}

/**
 * Copy another color into this one
 *
 * @param p_copy    The other color to copy.
 *
 * @return          The reference to color after copy.
 */
FORCEINLINE Color32&    Color32::operator=  (Color32 const& p_copy)
{
    m_r = p_copy.m_r;
    m_g = p_copy.m_g;
    m_b = p_copy.m_b;
    m_a = p_copy.m_a;

    return *this;
}

/**
 * Copy the Color into this color
 * (Converts the float value component into a 8 bits values)
 *
 * @param p_color   The Color to copy.
 *
 * @return          The reference to color after copy.
 */
FORCEINLINE Color32&    Color32::operator=  (Color const&   p_color)
{
    m_r = (uint8)(p_color.m_r / 255.0f);
    m_g = (uint8)(p_color.m_g / 255.0f);
    m_b = (uint8)(p_color.m_b / 255.0f);
    m_a = (uint8)(p_color.m_a / 255.0f);

    return *this;
}

/**
 * Move another color into this one
 *
 * @param p_move    The color to move.
 *
 * @return          The reference to color after move.
 */
FORCEINLINE Color32&    Color32::operator=  (Color32&&      p_move)
{
    m_r = std::move(p_move.m_r);
    m_g = std::move(p_move.m_g);
    m_b = std::move(p_move.m_b);
    m_a = std::move(p_move.m_a);

    return *this;
}

/**
 * Gets specific component of the color by index.
 *
 * @param p_index   The index of color component. Must be int [0,3] range.
 *
 * @return          The copy of the component corresponding to the index given.
 */
FORCEINLINE uint8       Color32::operator[] (int32 const    p_index)    const
{
    if (p_index < 0 || 3 < p_index)
        throw std::out_of_range("p_index is out of Color32's index range : [0,3]");

    if      (p_index == 0)
        return m_r;
    else if (p_index == 1)
        return m_g;
    else if (p_index == 2)
        return m_b;
    else
        return m_a;
}

/**
 * Gets specific component of the color.
 *
 * @param p_index   The index of color component. Must be int [0,3] range.
 *
 * @return          The reference to component corresponding to the index given.
 */
FORCEINLINE uint8&      Color32::operator[] (int32 const    p_index)
{
    if (p_index < 0 || 3 < p_index)
        throw std::out_of_range("p_index is out of Color32's index range : [0,3]");

    if      (p_index == 0)
        return m_r;
    else if (p_index == 1)
        return m_g;
    else if (p_index == 2)
        return m_b;
    else
        return m_a;
}

/**
 * Gets the result of component-wise addition of this and another color.
 *
 * @param p_color   The color to add to this.
 *
 * @return          The result of color addition.
 */
FORCEINLINE Color32     Color32::operator+  (Color32 const& p_color)    const
{
    return Color32
    (
        (uint8) Math::Min((int32)m_r + (int32)p_color.m_r, 255), 
        (uint8) Math::Min((int32)m_g + (int32)p_color.m_g, 255),
        (uint8) Math::Min((int32)m_b + (int32)p_color.m_b, 255),
        (uint8) Math::Min((int32)m_a + (int32)p_color.m_a, 255)
    );
}

/**
 * Adds another color to this.
 * Uses component-wise addition.
 *
 * @param p_color   Color32 to add to this.
 *
 * @return          Copy of the color after addition.
 */
FORCEINLINE Color32&    Color32::operator+= (Color32 const& p_color)
{
    m_r = (uint8)Math::Min((int32)m_r + (int32)p_color.m_r, 255);
    m_g = (uint8)Math::Min((int32)m_g + (int32)p_color.m_g, 255);
    m_b = (uint8)Math::Min((int32)m_b + (int32)p_color.m_b, 255);
    m_a = (uint8)Math::Min((int32)m_a + (int32)p_color.m_a, 255);

    return *this;
}

/**
 * Gets the result of component-wise subtraction of this by another color.
 *
 * @param p_color   The color to subtract from this.
 *
 * @return          The result of color subtraction.
 */
FORCEINLINE Color32     Color32::operator-  (Color32 const& p_color)    const
{
    return Color32
    (
        (uint8) Math::Max((int32)m_r - (int32)p_color.m_r, 0), 
        (uint8) Math::Max((int32)m_g - (int32)p_color.m_g, 0),
        (uint8) Math::Max((int32)m_b - (int32)p_color.m_b, 0),
        (uint8) Math::Max((int32)m_a - (int32)p_color.m_a, 0)
    );
}

/**
 * Subtracts another color from this.
 * Uses component-wise subtraction.
 *
 * @param p_color   The color to subtract from this.
 *
 * @return          The reference of the color after subtraction.
 */
FORCEINLINE Color32&    Color32::operator-= (Color32 const& p_color)
{
    m_r = (uint8)Math::Max((int32)m_r - (int32)p_color.m_r, 0);
    m_g = (uint8)Math::Max((int32)m_g - (int32)p_color.m_g, 0);
    m_b = (uint8)Math::Max((int32)m_b - (int32)p_color.m_b, 0);
    m_a = (uint8)Math::Max((int32)m_a - (int32)p_color.m_a, 0);

    return *this;
}

#endif // !__COLOR_32_INL__