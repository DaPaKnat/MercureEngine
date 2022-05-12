#ifndef __MATH_MACRO_HPP__
#define __MATH_MACRO_HPP__

/*************************************************
 *  Definitions of useful mathematical constants *
 *************************************************/

/*  Constant result of mathematical function */

#define MATH_EXP_F          (2.71828182845904523536F)           /*  exp(1)  */
#define MATH_EXP            (2.71828182845904523536)            /*  exp(1)  */
#define MATH_EXP_L          (2.71828182845904523536L)           /*  exp(1)  */

#define MATH_LOG2E_F        (1.44269504088896340736F)           /*  log2(e) */      
#define MATH_LOG2E          (1.44269504088896340736)            /*  log2(e) */      
#define MATH_LOG2E_L        (1.44269504088896340736L)           /*  log2(e) */      

#define MATH_LOG10E_F       (0.43429448190325182765F)           /*  log10(exp(1)))  */
#define MATH_LOG10E         (0.43429448190325182765)            /*  log10(exp(1)))  */
#define MATH_LOG10E_L       (0.43429448190325182765L)           /*  log10(exp(1)))  */

#define MATH_LN2_F          (0.69314718055994530948F)           /*  ln(2)   */
#define MATH_LN2            (0.69314718055994530948)            /*  ln(2)   */
#define MATH_LN2_L          (0.69314718055994530948L)           /*  ln(2)   */

#define MATH_LN10_F         (2.30258509299404568402F)           /*  ln(10)  */
#define MATH_LN10           (2.30258509299404568402)            /*  ln(10)  */
#define MATH_LN10_L         (2.30258509299404568402L)           /*  ln(10)  */

/* Constant trigonometry value */

#define MATH_TAU_F          (6.28318530717958647692F)           /*  tau = 2π    */
#define MATH_TAU            (6.28318530717958647692)            /*  tau = 2π    */
#define MATH_TAU_L          (6.28318530717958647692L)           /*  tau = 2π    */

#define MATH_PI_F           (3.14159265358979323846F)           /*  π   */
#define MATH_PI             (3.14159265358979323846)            /*  π   */
#define MATH_PI_L           (3.14159265358979323846L)           /*  π   */

#define MATH_2PI_F          (MATH_TAU_F)                        /*  2π  */
#define MATH_2PI            (MATH_TAU)                          /*  2π  */
#define MATH_2PI_L          (MATH_TAU_L)                        /*  2π  */

#define MATH_PI_2_F         (1.57079632679489661923F)           /*  π/2 = 2π/4 = 3π/6   */
#define MATH_PI_2           (1.57079632679489661923 )           /*  π/2 = 2π/4 = 3π/6   */
#define MATH_PI_2_L         (1.57079632679489661923L)           /*  π/2 = 2π/4 = 3π/6   */

#define MATH_PI_3_F         (1.04719755119659774615F)           /*  π/3 = 2π/6  */
#define MATH_PI_3           (1.04719755119659774615)            /*  π/3 = 2π/6  */
#define MATH_PI_3_L         (1.04719755119659774615L)           /*  π/3 = 2π/6  */

#define MATH_PI_4_F         (0.78539816339744830962F)           /*  π/4 */
#define MATH_PI_4           (0.78539816339744830962)            /*  π/4 */
#define MATH_PI_4_L         (0.78539816339744830962L)           /*  π/4 */

#define MATH_PI_6_F         (0.52359877559829887308F)           /*  π/6 */
#define MATH_PI_6           (0.52359877559829887308)            /*  π/6 */
#define MATH_PI_6_L         (0.52359877559829887308L)           /*  π/6 */

#define MATH_2_PI_3_F       (2.09439510239319549231F)           /*  2π/3    */
#define MATH_2_PI_3         (2.09439510239319549231)            /*  2π/3    */
#define MATH_2_PI_3_L       (2.09439510239319549231L)           /*  2π/3    */

#define MATH_3_PI_4_F       (2.35619449019234492885F)           /*  3π/4    */
#define MATH_3_PI_4         (2.35619449019234492885)            /*  3π/4    */
#define MATH_3_PI_4_L       (2.35619449019234492885L)           /*  3π/4    */

#define MATH_5_PI_6_F       (2.61799387799149436538F)           /*  5π/6    */
#define MATH_5_PI_6         (2.61799387799149436538)            /*  5π/6    */
#define MATH_5_PI_6_L       (2.61799387799149436538L)           /*  5π/6    */

#define MATH_1_PI_F         (0.31830988618379067154F)           /*  1/π */
#define MATH_1_PI           (0.31830988618379067154)            /*  1/π */
#define MATH_1_PI_L         (0.31830988618379067154L)           /*  1/π */

#define MATH_2_PI_F         (0.63661977236758134308F)           /*  2/π */
#define MATH_2_PI           (0.63661977236758134308)            /*  2/π */
#define MATH_2_PI_L         (0.63661977236758134308L)           /*  2/π */

#define MATH_SQRT_PI_F      (1.77245385090551602730F)           /*  sqrt(π) */
#define MATH_SQRT_PI        (1.77245385090551602730)            /*  sqrt(π) */
#define MATH_SQRT_PI_L      (1.77245385090551602730L)           /*  sqrt(π) */

#define MATH_2_SQRT_PI_F    (1.12837916709551257390F)           /*  2/sqrt(π)   */
#define MATH_2_SQRT_PI      (1.12837916709551257390)            /*  2/sqrt(π)   */
#define MATH_2_SQRT_PI_L    (1.12837916709551257390L)           /*  2/sqrt(π)   */

#define MATH_SQRT_2_F       (1.41421356237309504880F)           /*  sqrt(2) */
#define MATH_SQRT_2         (1.41421356237309504880)            /*  sqrt(2) */
#define MATH_SQRT_2_L       (1.41421356237309504880L)           /*  sqrt(2) */

#define MATH_1_SQRT_2_F     (0.70710678118654752440F)           /*  1/sqrt(2) = sqrt(2)/2   */
#define MATH_1_SQRT_2       (0.70710678118654752440)            /*  1/sqrt(2) = sqrt(2)/2   */
#define MATH_1_SQRT_2_L     (0.70710678118654752440L)           /*  1/sqrt(2) = sqrt(2)/2   */

#define MATH_SQRT_3_2_F     (0.8660254037844386F)               /*  sqrt(3)/2   */
#define MATH_SQRT_3_2       (0.8660254037844386)                /*  sqrt(3)/2   */
#define MATH_SQRT_3_2_L     (0.8660254037844386L)               /*  sqrt(3)/2   */

#define MATH_RAD_2_DEG_F    (57.2957795130823208768F)           /*  180/π   */
#define MATH_RAD_2_DEG      (57.2957795130823208768)            /*  180/π   */
#define MATH_RAD_2_DEG_L    (57.2957795130823208768L)           /*  180/π   */

#define MATH_DEG_2_RAD_F    (0.01745329251994329577F)           /*  π/180   */
#define MATH_DEG_2_RAD      (0.01745329251994329577)            /*  π/180   */
#define MATH_DEG_2_RAD_L    (0.01745329251994329577L)           /*  π/180   */

/*
Useful specificity of floating point value
*/

#define MATH_EPSILON_F      (1.192092896e-07F)                  /* smallest number such that 1.0F + DBL_EPSILON != 1.0  */
#define MATH_EPSILON        (2.2204460492503131e-016)           /* smallest number such that 1.0  + DBL_EPSILON != 1.0  */
#define MATH_EPSILON_L      (2.2204460492503131e-016L)          /* smallest number such that 1.0L + DBL_EPSILON != 1.0  */

#define MATH_INFINITY_F     (float)(1e+300 * 1e+300)            /* A representation of positive infinity (float value)          */
#define MATH_INFINITY       (1e+300*1e+300)                     /* A representation of positive infinity (double value)         */
#define MATH_INFINITY_L     (long double)(1e+300L * 1e+300L)    /* A representation of positive infinity (long double value)    */

#define MATH_NINFINITY_F    (-MATH_INFINITY_F)                  /* A representation of negatove infinity (float value)          */
#define MATH_NINFINITY      (-MATH_INFINITY)                    /* A representation of negatove infinity (double value)         */
#define MATH_NINFINITY_L    (-MATH_INFINITY_L)                  /* A representation of negatove infinity (long double value)    */

#define MATH_NAN_F          (MATH_INFINITY_F * 0.0f)            /* Not a number value as float  */
#define MATH_NAN            (MATH_INFINITY * 0.0)               /* Not a number value as double */
#define MATH_NAN_L          (MATH_INFINITY_L * 0.0l)            /* Not a number value as long double */

#endif // !__MATH_MACRO_HPP__