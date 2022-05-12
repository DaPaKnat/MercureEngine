#ifndef __COLOR_INL__
#define __COLOR_INL__

#include "Color32.hpp"

/*==============================[Public Static Methods]==============================*/

/**
 * Clamps the color given between two others color min and max inclued
 * (Component-wise clamp for Color)
 *
 * @param p_color   The color to clamp.
 * @param p_min     The minimum color (Each components corresponding to minimum).
 * @param p_max     The maximum color (Each components corresponding to maximum).
 *
 * @return          The color clamped.
 */
FORCEINLINE Color   Color::Clamp            (Color const&   p_color,
                                             Color const&   p_min,
                                             Color const&   p_max)
{
    return Color
    (
        Math::Clamp(p_color.m_r, p_min.m_r, p_max.m_r),
        Math::Clamp(p_color.m_g, p_min.m_g, p_max.m_g),
        Math::Clamp(p_color.m_b, p_min.m_b, p_max.m_b),
        Math::Clamp(p_color.m_a, p_min.m_a, p_max.m_a)
    );
}

/**
 * Clamps the components of the color given between two values min and max inclued
 *
 * @param p_color   The color to clamp.
 * @param p_min     The minimum value (Default = 0.0f).
 * @param p_max     The maximum value (Default = 1.0f).
 *
 * @return          The color clamped.
 */
FORCEINLINE Color   Color::Clamp            (Color const&   p_color,
                                             float const    p_min,
                                             float const    p_max)
{
    return Color
    (
        Math::Clamp(p_color.m_r, p_min, p_max),
        Math::Clamp(p_color.m_g, p_min, p_max),
        Math::Clamp(p_color.m_b, p_min, p_max),
        Math::Clamp(p_color.m_a, p_min, p_max)
    );
}

/**
 * Check if two colors are nearly equal within a specified tolerance.
 *
 * @param p_vector      The first color to check against.
 * @param p_vector      The second colorr to check against.
 * @param p_tolerance   The error tolerance.
 *
 * @return              True if the color are equal within tolerance limits, false otherwise.
 */
FORCEINLINE bool    Color::IsNearlyEqual    (Color const&   p_a,
                                             Color const&   p_b,
                                             float const    p_tolerance)
{
    return  Math::IsNearlyEqual(p_a.m_r, p_b.m_r, p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_g, p_b.m_g, p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_b, p_b.m_b, p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_a, p_b.m_a, p_tolerance);
}

/**
 * Linearly interpolates between two colors (p_alpha is not clamped between 0 and 1)
 *
 * @param p_a       The left handed color
 * @param p_b       The right handed color
 * @param p_alpha   The interpolation value
 *
 * @return          The interpolation of p_a and p_b by p_alpha
 */
FORCEINLINE Color   Color::Lerp             (Color const&   p_a,
                                             Color const&   p_b,
                                             float const    p_alpha)
{
    return Color
    (
        Math::Lerp(p_a.m_r, p_b.m_r, p_alpha),
        Math::Lerp(p_a.m_g, p_b.m_g, p_alpha),
        Math::Lerp(p_a.m_b, p_b.m_b, p_alpha),
        Math::Lerp(p_a.m_a, p_b.m_a, p_alpha)
    );
}

/*==============================[Constructors + Destructor]==============================*/

/** Default constructor, all components are initialized to 0.0f except alpha = 1.0f */
FORCEINLINE Color::Color    ()
:   m_r {0.0f},
    m_g {0.0f},
    m_b {0.0f},
    m_a {1.0f}
{}

/**
 * Constructor using initial values for each component.
 *
 * @param p_r   The red component. Must be in range [0,1].
 * @param p_g   The green component. Must be in range [0,1].
 * @param p_b   The blue component. Must be in range [0,1].
 * @param p_a   The alpha component. Must be in range [0,1].
 */
FORCEINLINE Color::Color    (float const    p_r,
                             float const    p_g,
                             float const    p_b,
                             float const    p_a)
:   m_r {p_r},
    m_g {p_g},
    m_b {p_b},
    m_a {p_a}
{}

/**
 * Constructor using initial values for each component.
 *
 * @param p_r   The red component. Must be in range [0,255].
 * @param p_g   The green component. Must be in range [0,255].
 * @param p_b   The blue component. Must be in range [0,255].
 * @param p_a   The alpha component. Must be in range [0,255].
 */
FORCEINLINE Color::Color    (uint8 const    p_r,
                             uint8 const    p_g,
                             uint8 const    p_b,
                             uint8 const    p_a)
:   m_r {p_r/255.0f},
    m_g {p_g/255.0f},
    m_b {p_b/255.0f},
    m_a {p_a/255.0f}
{}

/**
 * Constructor using one vector 3D for the RGB components and one value for the alpha.
 *
 * @param p_rgb  The 3D vector to RGB components.
 * @param p_a    The Alpha component (p_a = 1.0f by default).
 */
FORCEINLINE Color::Color    (Vector3 const& p_rgb, 
                             float const    p_a)
:   m_r {p_rgb.m_x},
    m_g {p_rgb.m_y},
    m_b {p_rgb.m_z},
    m_a {p_a}
{}

/**
 * Constructor using one vector 4D for the RGBA components.
 *
 * @param p_rgba The 4D vector to RGBA components.
 */
FORCEINLINE Color::Color    (Vector4 const& p_rgba)
:   m_r {p_rgba.m_x},
    m_g {p_rgba.m_y},
    m_b {p_rgba.m_z},
    m_a {p_rgba.m_w}
{}

/**
 * Constructor which convert a Color32 to a Color.
 *
 * @param p_color   The Color32 to convert to Color.
 */
FORCEINLINE Color::Color    (Color32 const& p_color32)
:   m_r {p_color32.m_r/255.0f},
    m_g {p_color32.m_g/255.0f},
    m_b {p_color32.m_b/255.0f},
    m_a {p_color32.m_a/255.0f}
{}

/**
 * Copy constructor using initial color for each component.
 *
 * @param p_copy    The initial color to copy.
 */
FORCEINLINE Color::Color    (Color const&   p_copy)
:   m_r {p_copy.m_r},
    m_g {p_copy.m_g},
    m_b {p_copy.m_b},
    m_a {p_copy.m_a}
{}

/**
* Move constructor using initial Color32 for each component.
*
* @param p_move    The initial Color32 to p_move.
*/
FORCEINLINE Color::Color    (Color&&        p_move)
:   m_r {std::move(p_move.m_r)},
    m_g {std::move(p_move.m_g)},
    m_b {std::move(p_move.m_b)},
    m_a {std::move(p_move.m_a)}
{}

/*==============================[Public Local Methods]==============================*/

/**
 * Gets a specific component of the color by index.
 *
 * @param p_index   The index of the component required.
 *
 * @return          The copy of the specified component.
 */
FORCEINLINE float   Color::Component    (int32 const    p_index)    const
{
    if (p_index < 0 || 3 < p_index)
        throw std::out_of_range("p_index is out of Colors's index range : [0,3]");

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
FORCEINLINE float&  Color::Component    (int32 const    p_index)
{
    if (p_index < 0 || 3 < p_index)
        throw std::out_of_range("p_index is out of Colors's index range : [0,3]");

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
 * Get the maximum value of the Color's components.
 *
 * @return  The maximum value of the color's components.
 */
FORCEINLINE float   Color::GetMax       ()                          const
{
    return Math::Max(m_r,Math::Max(m_g, Math::Max(m_b,m_a)));
}

/**
 * Get the maximum value of the Color's RGB components.
 *
 * @return  The maximum value of the RGB color's components.
 */
FORCEINLINE float   Color::GetMaxRGB    ()                          const
{
    return Math::Max3(m_r, m_g, m_b);
}

/**
 * Get the minimum value of the Color's RGB components.
 *
 * @return  The minimum value of the RGB color's components.
 */
FORCEINLINE float   Color::GetMin       ()                          const
{
    return Math::Min(m_r,Math::Min(m_g, Math::Min(m_b,m_a)));
}

/**
 * Get the minimum value of the Color's components.
 *
 * @return  The minimum value of the color's components.
 */
FORCEINLINE float   Color::GetMinRGB    ()                          const
{
    return Math::Min3(m_r, m_g, m_b);
}

/**
 * Creater Color32 with this color
 *
 * @return This color converted into a Color32.
 */
FORCEINLINE Color32 Color::ToColor32    ()                          const
{
    return Color32
    (
        (uint8)(Math::Clamp01(m_r) * 255),
        (uint8)(Math::Clamp01(m_g) * 255),
        (uint8)(Math::Clamp01(m_b) * 255),
        (uint8)(Math::Clamp01(m_a) * 255)
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
FORCEINLINE bool    Color::operator==   (Color const&   p_color)    const
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
FORCEINLINE bool    Color::operator!=   (Color const&   p_color)    const
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
FORCEINLINE Color&  Color::operator=    (Color const&   p_copy)
{
    m_r = p_copy.m_r;
    m_g = p_copy.m_g;
    m_b = p_copy.m_b;
    m_a = p_copy.m_a;

    return *this;
}

/**
 * Copy the Color32 into this color
 * (Converts the 8-bits component into a float values)
 *
 * @param p_color32 The Color32 to copy.
 *
 * @return          The reference to color after copy.
 */
FORCEINLINE Color&  Color::operator=    (Color32 const& p_color32)
{
    m_r = p_color32.m_r / 255.0f;
    m_g = p_color32.m_g / 255.0f;
    m_b = p_color32.m_b / 255.0f;
    m_a = p_color32.m_a / 255.0f;

    return *this;
}

/**
 * Move another color into this one
 *
 * @param p_move    The color to move.
 *
 * @return          The reference to color after move.
 */
FORCEINLINE Color&  Color::operator=    (Color&&        p_move)
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
FORCEINLINE float   Color::operator[]   (int32 const    p_index)    const
{
    if (p_index < 0 || 3 < p_index)
        throw std::out_of_range("p_index is out of Colors's index range : [0,3]");

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
FORCEINLINE float&  Color::operator[]   (int32 const    p_index)
{
    if (p_index < 0 || 3 < p_index)
        throw std::out_of_range("p_index is out of Colors's index range : [0,3]");

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
FORCEINLINE Color   Color::operator+    (Color const&   p_color)    const
{
    return Color
    (
        m_r + p_color.m_r, 
        m_g + p_color.m_g,
        m_b + p_color.m_b,
        m_a + p_color.m_a
    );
}

/**
 * Adds another color to this.
 * Uses component-wise addition.
 *
 * @param p_color   Color to add to this.
 *
 * @return          Copy of the color after addition.
 */
FORCEINLINE Color&  Color::operator+=   (Color const&   p_color)
{
    m_r += p_color.m_r;
    m_g += p_color.m_g;
    m_b += p_color.m_b;
    m_a += p_color.m_a;

    return *this;
}

/**
 * Gets the result of component-wise subtraction of this by another color.
 *
 * @param p_color   The color to subtract from this.
 *
 * @return          The result of color subtraction.
 */
FORCEINLINE Color   Color::operator-    (Color const&   p_color)    const
{
    return Color
    (
        m_r - p_color.m_r,
        m_g - p_color.m_g,
        m_b - p_color.m_b,
        m_a - p_color.m_a
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
FORCEINLINE Color&  Color::operator-=   (Color const&   p_color)
{
    m_r -= p_color.m_r;
    m_g -= p_color.m_g;
    m_b -= p_color.m_b;
    m_a -= p_color.m_a;

    return *this;
}

/**
 * Gets the result of component-wise multiplication of this color by another.
 *
 * @param p_color   The color to multiply with.
 *
 * @return          The result of multiplication.
 */
FORCEINLINE Color   Color::operator*    (Color const&   p_color)    const
{
    return Color
    (
        m_r * p_color.m_a, 
        m_g * p_color.m_g,
        m_b * p_color.m_b,
        m_a * p_color.m_a
    );
}

/**
 * Gets the result of scaling the color (multiplying each component by a value).
 *
 * @param p_scale   What to multiply each component by.
 *
 * @return          The result of multiplication.
 */
FORCEINLINE Color   Color::operator*    (float const    p_scale)    const
{
    return Color
    (
        m_r * p_scale, 
        m_g * p_scale,
        m_b * p_scale,
        m_a * p_scale
    );    
}

/**
 * Multiplies the color with another color, using component-wise multiplication.
 *
 * @param p_color   What to multiply this color with.
 *
 * @return          The reference of the color after multiplication.
 */
FORCEINLINE Color&  Color::operator*=   (Color const&   p_color)
{
    m_r *= p_color.m_r;
    m_g *= p_color.m_g;
    m_b *= p_color.m_b;
    m_a *= p_color.m_a;

    return *this;
}

/**
 * Multiply the color by a value.
 * Multiply each component of the color by a number
 *
 * @param p_scale   What to multiply each component by.
 *
 * @return          The reference of the color after multiplication.
 */
FORCEINLINE Color&  Color::operator*=   (float const    p_scale)
{
    m_r *= p_scale;
    m_g *= p_scale;
    m_b *= p_scale;
    m_a *= p_scale;

    return *this;
}

/**
 * Gets the result of component-wise division of this color by another.
 *
 * @param p_color   The color to divide by.
 *
 * @return          The result of division.
 */
FORCEINLINE Color   Color::operator/    (Color const&   p_color)    const
{
    return Color
    (
        m_r / p_color.m_r,
        m_g / p_color.m_g,
        m_b / p_color.m_b,
        m_a / p_color.m_a
    );
}

/**
 * Gets the result of dividing each component of the color by a value.
 *
 * @param p_scale   What to divide each component by.
 *
 * @return          The result of division.
 */
FORCEINLINE Color   Color::operator/    (float const    p_scale)    const
{
    return Color
    (
        m_r / p_scale, 
        m_g / p_scale,
        m_b / p_scale,
        m_a / p_scale
    );      
}

/**
 * Divides this color by another colo, using component-wise division.
 *
 * @param p_color   The color to divide by.
 *
 * @return          The reference of the color after division.
 */
FORCEINLINE Color&  Color::operator/=   (Color const&   p_color)
{
    m_r /= p_color.m_r;
    m_g /= p_color.m_g;
    m_b /= p_color.m_b;
    m_a /= p_color.m_a;

    return *this;
}

/**
 * Divides the color by a number.
 * Divides each component of the vector by a number
 *
 * @param p_scale   What to divide each component by.
 *
 * @return          The reference of the color after division.
 */
FORCEINLINE Color&  Color::operator/=   (float const    p_scale)
{
    m_r /= p_scale;
    m_g /= p_scale;
    m_b /= p_scale;
    m_a /= p_scale;

    return *this;
}

/*==============================[Public Globale Operators]==============================*/

/**
 * Multiplies a color by a scaling factor.
 *
 * @param p_scale   The scaling factor.
 * @param p_color   The color to scale.
 *
 * @return          The result of multiplication.
 */
FORCEINLINE Color           operator*   (float const    p_scale,
                                     Color const&   p_color)
{
    return Color
    (
        p_color.m_r * p_scale, 
        p_color.m_g * p_scale,
        p_color.m_b * p_scale,
        p_color.m_a * p_scale
    ); 
}

#endif // !__COLOR_INL__