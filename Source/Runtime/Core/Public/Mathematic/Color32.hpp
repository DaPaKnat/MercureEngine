#ifndef __COLOR_32_HPP__
#define __COLOR_32_HPP__

//#include "MathMacro.hpp"

#include "HAL/Platform.hpp"
#include "Reflection/Reflection.hpp"

//=========================[Forward Declaration]=========================//

    struct Color;

//=======================================================================//

/**
 * Representation of RGBA color in 32 bit format. 
 * Each Color component is a byte value with a range from 0 to 255.
 */
struct ENGINE_API Color32
{
	REFLECT(Color32)

    //=========================[Public Static Properties]=========================//
    
        /** Black Color (0,0,0,255) */
        static  Color32 const   Black;

        /** Blue Color (0,0,255,255) */
        static  Color32 const   Blue;

        /** Clear Color (0,0,0,0) */
        static  Color32 const   Clear;

        /** Cyan Color (0,255,255,255) */
        static  Color32 const   Cyan;

        /** Gray Color (127,127,127,255) */
        static  Color32 const   Grey;

        /** Green Color (0,255,0,255) */
        static  Color32 const   Green;

        /** Magenta Color (255,0,255,255) */
        static  Color32 const   Magenta;

        /** Red Color (255,0,0,255) */
        static  Color32 const   Red;

        /** White Color (255,255,255,255) */
        static  Color32 const   White;

        /** Yellow Color (255,255,0,255) */
        static  Color32 const   Yellow;

   //=========================[Public Local Properties]=========================//

        union 
        {
            struct
            {
                /** Alpha component of the color (0 is transparent and 255 is opaque) */
                uint8   m_a;

                /** Blue component of the color */
                uint8   m_b;

                /** Green component of the color */
                uint8   m_g;

                /** Red component of the color */
                uint8   m_r;
            };

                /** Integer corresponding to a color */
				PROPERTY()
                uint32  m_rgba;
        };

    //=========================[Public Static Methods]=========================//

        static  FORCEINLINE Color32 Clamp   (Color32 const& p_color,
                                             Color32 const& p_min,
                                             Color32 const& p_max);

        static  FORCEINLINE Color32 Lerp    (Color32 const& p_a,
                                             Color32 const& p_b,
                                            float const    p_alpha);

    //=========================[Constructor + Destructor]=========================//

        FORCEINLINE Color32     ();

        FORCEINLINE Color32     (uint32 const   p_rgba);

        FORCEINLINE Color32     (uint8 const    p_r,
                                 uint8 const    p_g,
                                 uint8 const    p_b,
                                 uint8 const    p_a = 255);

        FORCEINLINE Color32     (Color const&   p_color);

        FORCEINLINE Color32     (Color32 const& p_copy);

        FORCEINLINE Color32     (Color32&&      p_move);

                    ~Color32    () = default;

    //=========================[Public Local Methods]=========================//

        FORCEINLINE uint8   Component   (int32 const    p_index)    const;

        FORCEINLINE uint8&  Component   (int32 const    p_index);

        FORCEINLINE uint8   GetMax      ()                          const;

        FORCEINLINE uint8   GetMaxRGB   ()                          const;

        FORCEINLINE uint8   GetMin      ()                          const;

        FORCEINLINE uint8   GetMinRGB   ()                          const;

        FORCEINLINE Color   ToColor     ()                          const;

    //=========================[Public Local Operators]=========================//

        FORCEINLINE bool        operator==  (Color32 const& p_color)    const;
        FORCEINLINE bool        operator!=  (Color32 const& p_color)    const;

        FORCEINLINE Color32&    operator=   (Color32 const& p_copy);
        FORCEINLINE Color32&    operator=   (Color32&&      p_move);
        FORCEINLINE Color32&    operator=   (Color const&   p_color);

        FORCEINLINE uint8       operator[]  (int32 const    p_index)    const;
        FORCEINLINE uint8&      operator[]  (int32 const    p_index);

        FORCEINLINE Color32     operator+   (Color32 const& p_color)    const;
        FORCEINLINE Color32&    operator+=  (Color32 const& p_color);

        FORCEINLINE Color32     operator-   (Color32 const& p_color)    const;
        FORCEINLINE Color32&    operator-=  (Color32 const& p_color);

}; // !struct Color32

#include "Color32.inl"
#include "Color32.generated.hpp"

#endif // !__COLOR_32_HPP__