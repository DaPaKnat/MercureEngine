#ifndef __COLOR_HPP__
#define __COLOR_HPP__

#include "Vector4.hpp"

//=========================[Forward Declaration]=========================//

    struct Color32;

//=======================================================================//

/**
 * Representation of RGBA color. 
 * Each Color component is a floating point value with a range from 0.0f to 1.0f.
 */
struct ENGINE_API Color
{
	REFLECT(Color)

    //=========================[Public Static Properties]=========================//

        /** Black Color (0,0,0,1) */
        static  Color const Black;

        /** Blue Color (0,0,1,1) */
        static  Color const Blue;

        /** Clear Color (0,0,0,0) */
        static  Color const Clear;

        /** Cyan Color (0,1,1,1) */
        static  Color const Cyan;

        /** Gray Color (0.5,0.5,0.5,1) */
        static  Color const Grey;

        /** Green Color (0,1,0,1) */
        static  Color const Green;

        /** Magenta Color (1,0,1,1) */
        static  Color const Magenta;

        /** Red Color (1,0,0,1) */
        static  Color const Red;

        /** White Color (1,1,1,1) */
        static  Color const White;

        /** Yellow Color (1,1,0,1) */
        static  Color const Yellow;

    //=========================[Public Static Methods]=========================//

        static  FORCEINLINE Color   Clamp           (Color const&   p_color,
                                                     Color const&   p_min,
                                                     Color const&   p_max);

        static  FORCEINLINE Color   Clamp           (Color const&   p_color,
                                                     float const    p_min = 0.0f,
                                                     float const    p_max = 1.0f);

        static  FORCEINLINE bool    IsNearlyEqual   (Color const&   p_a,
                                                     Color const&   p_b,
                                                     float const    p_tolerance = MATH_EPSILON_F);

        static  FORCEINLINE Color   Lerp            (Color const&   p_a,
                                                     Color const&   p_b,
                                                     float const    p_alpha);

    //=========================[Public Local Properties]=========================//

        /** Red component of the color */
		PROPERTY()
        float    m_r;

        /** Green component of the color */
		PROPERTY()
        float    m_g;

        /** Blue component of the color */
		PROPERTY()
        float    m_b;

        /** Alpha component of the color (0 is transparent and 1 is opaque) */
		PROPERTY()
        float    m_a;

    //=========================[Constructor + Destructor]=========================//

        FORCEINLINE Color   ();

        FORCEINLINE Color   (float const    p_r,
                             float const    p_g,
                             float const    p_b,
                             float const    p_a = 1.0f);

        FORCEINLINE Color   (uint8 const    p_r,
                             uint8 const    p_g,
                             uint8 const    p_b,
                             uint8 const    p_a = 255);

        FORCEINLINE Color   (Vector3 const& p_rgb,
                             float const    p_a);

        FORCEINLINE Color   (Vector4 const& p_rgba);

        FORCEINLINE Color   (Color32 const& p_color32);

        FORCEINLINE Color   (Color const&   p_copy);

        FORCEINLINE Color   (Color&&        p_move);

                    ~Color  () = default;

    //=========================[Public Local Methods]=========================//

        FORCEINLINE float   Component   (int32 const    p_index)    const;

        FORCEINLINE float&  Component   (int32 const    p_index);

        FORCEINLINE float   GetMax      ()                          const;

        FORCEINLINE float   GetMaxRGB   ()                          const;

        FORCEINLINE float   GetMin      ()                          const;

        FORCEINLINE float   GetMinRGB   ()                          const;

        FORCEINLINE Color32 ToColor32   ()                          const;

    //=========================[Public Local Operators]=========================//

        FORCEINLINE bool    operator==  (Color const&   p_color)    const;
        FORCEINLINE bool    operator!=  (Color const&   p_color)    const;

        FORCEINLINE Color&  operator=   (Color const&   p_copy);
        FORCEINLINE Color&  operator=   (Color&&        p_move);
        FORCEINLINE Color&  operator=   (Color32 const& p_color32);

        FORCEINLINE float   operator[]  (int32 const    p_index)    const;
        FORCEINLINE float&  operator[]  (int32 const    p_index);

        FORCEINLINE Color   operator+   (Color const&   p_color)    const;
        FORCEINLINE Color&  operator+=  (Color const&   p_color);

        FORCEINLINE Color   operator-   (Color const&   p_color)    const;
        FORCEINLINE Color&  operator-=  (Color const&   p_color);

        FORCEINLINE Color   operator*   (Color const&   p_color)    const;
        FORCEINLINE Color   operator*   (float const    p_scale)    const;
        FORCEINLINE Color&  operator*=  (Color const&   p_color);
        FORCEINLINE Color&  operator*=  (float const    p_scale);

        FORCEINLINE Color   operator/   (Color const&   p_color)    const;
        FORCEINLINE Color   operator/   (float const    p_scale)    const;
        FORCEINLINE Color&  operator/=  (Color const&   p_color);
        FORCEINLINE Color&  operator/=  (float const    p_scale);

}; // !struct Color

/*==============================[Public Globale Operators]==============================*/
    
    FORCEINLINE Color   operator*   (float const    p_scale,
                                     Color const&   p_color);
    
#include "Color.inl"
#include "Color.generated.hpp"

#endif // !__COLOR_HPP__