#ifndef __MATRIX_4X4_HPP__
#define __MATRIX_4X4_HPP__

#include "Quaternion.hpp"
#include "Vector4.hpp"


/**
 * 4x4 matrix of floating point values.
 *
 * In order to access to elements use operator(RowIndex,ColumnIndex)
 *
 * Matrix convention: Column-Major
 *
 *  | 0 , 4 , 8  , 12 | | 00 , 01 , 02  , 03 |
 *  | 1 , 5 , 9  , 13 | | 10 , 11 , 12  , 13 |
 *  | 2 , 6 , 10 , 14 | | 20 , 21 , 22  , 23 |
 *  | 3 , 7 , 11 , 15 | | 30 , 31 , 32  , 33 |
 *
 */
class ENGINE_API Matrix4x4
{
	REFLECT(Matrix4x4)

    public:
    
    //=========================[Public Static Properties]=========================//
        
        /**
         *  Identity Matrix:
         *
         *  |   1.0f,   0.0f,   0.0f,   0.0f    |
         *  |   0.0f,   1.0f,   0.0f,   0.0f    |
         *  |   0.0f,   0.0f,   1.0f,   0.0f    |
         *  |   0.0f,   0.0f,   0.0f,   1.0f    |
         *
         */
        static  const   Matrix4x4   Identity;

        /**
         *  Zero Matrix:
         *
         *  |   0.0f,   0.0f,   0.0f,   0.0f    |
         *  |   0.0f,   0.0f,   0.0f,   0.0f    |
         *  |   0.0f,   0.0f,   0.0f,   0.0f    |
         *  |   0.0f,   0.0f,   0.0f,   0.0f    |
         *
         */
        static  const   Matrix4x4   Zero;

    //=========================[Public Static Methods]=========================//        

        static  FORCEINLINE bool        IsNearlyEqual   (Matrix4x4 const&   p_a,
                                                         Matrix4x4 const&   p_b,
                                                         float const        p_tolerance = MATH_EPSILON_F);
        
        static  FORCEINLINE Matrix4x4   LookAt          (Vector3 const&     p_from,
                                                         Vector3 const&     p_to,
                                                         Vector3 const&     p_up);

        static  FORCEINLINE Matrix4x4   Ortho           (float const        p_left,
                                                         float const        p_right,
                                                         float const        p_bottom,
                                                         float const        p_top,
                                                         float const        p_near,
                                                         float const        p_far);

        static  FORCEINLINE Matrix4x4   Perspective     (float const        p_fov,
                                                         float const        p_aspect,
                                                         float const        p_near,
                                                         float const        p_far);

        static  FORCEINLINE Matrix4x4   Perspective     (float const        p_fov,
                                                         float const        p_wdith,
                                                         float const        p_height,
                                                         float const        p_near,
                                                         float const        p_far);

        static  FORCEINLINE Matrix4x4   Rotate          (Quaternion const&  p_q);
        
        static  FORCEINLINE Matrix4x4   RotationXDeg    (float const        p_angleDeg);
                                                   
        static  FORCEINLINE Matrix4x4   RotationXRad    (float const        p_angleRad);
                                                   
        static  FORCEINLINE Matrix4x4   RotationYDeg    (float const        p_angleDeg);
                                                   
        static  FORCEINLINE Matrix4x4   RotationYRad    (float const        p_angleRad);
                                                   
        static  FORCEINLINE Matrix4x4   RotationZDeg    (float const        p_angleDeg);
                                                   
        static  FORCEINLINE Matrix4x4   RotationZRad    (float const        p_angleRad);

        static  FORCEINLINE Matrix4x4   Scale           (Vector3 const&     p_scale);


        static  FORCEINLINE Matrix4x4   Translate       (Vector3 const&     p_translation);

        static  FORCEINLINE Matrix4x4   TRS             (Vector3 const&     p_translation,
                                                         Quaternion const&  p_q,
                                                         Vector3 const&     p_scale);

        static  FORCEINLINE Matrix4x4   View            (Vector3 const&     p_from,
                                                         Vector3 const&     p_to,
                                                         Vector3 const&     p_up);

    //=========================[Constructor + Destructor]=========================//

        FORCEINLINE Matrix4x4   ();

        FORCEINLINE Matrix4x4   (float const        p_m00, 
                                 float const        p_m01, 
                                 float const        p_m02, 
                                 float const        p_m03,
                                 float const        p_m10, 
                                 float const        p_m11, 
                                 float const        p_m12, 
                                 float const        p_m13,
                                 float const        p_m20, 
                                 float const        p_m21, 
                                 float const        p_m22, 
                                 float const        p_m23,
                                 float const        p_m30, 
                                 float const        p_m31, 
                                 float const        p_m32, 
                                 float const        p_m33);

        FORCEINLINE Matrix4x4   (Vector3 const&     p_columnX,
                                 Vector3 const&     p_columnY,
                                 Vector3 const&     p_columnZ,
                                 Vector3 const&     p_columnW);

        FORCEINLINE Matrix4x4   (Vector4 const&     p_columnX,
                                 Vector4 const&     p_columnY,
                                 Vector4 const&     p_columnZ,
                                 Vector4 const&     p_columnW);

        FORCEINLINE Matrix4x4   (Matrix4x4 const&   p_copy);

        FORCEINLINE Matrix4x4   (Matrix4x4&&        p_move);

                    ~Matrix4x4  () = default;

    //=========================[Public Local Methods]=========================//
        
        FORCEINLINE Vector3     ExtractScale            (float const    p_tolerance = MATH_EPSILON_F);

        FORCEINLINE Vector3     GetAxis                 (int32 const    p_index)                        const;

        FORCEINLINE Vector3     GetBackward             ()                                              const;

        FORCEINLINE Matrix4x4   GetCofactor             ()                                              const;

        FORCEINLINE Vector4     GetColumn               (int32 const    p_index)                        const;

        FORCEINLINE float       GetDeterminant          ()                                              const;
        
        FORCEINLINE Vector3     GetDown                 ()                                              const;

        FORCEINLINE Vector3     GetForward              ()                                              const;

        FORCEINLINE Matrix4x4   GetInverse              ()                                              const;

        FORCEINLINE Vector3     GetLeft                 ()                                              const;

        FORCEINLINE Matrix4x4   GetMatrixWithoutScale   (float const    p_tolerance = MATH_EPSILON_F)   const;

        FORCEINLINE float       GetMaximumAxisScale     ()                                              const;

        FORCEINLINE Vector3     GetOrigin               ()                                              const;

        FORCEINLINE Vector3     GetRight                ()                                              const;

        FORCEINLINE Quaternion  GetRotation             ()                                              const;

        FORCEINLINE float       GetRotationDeterminant  ()                                              const;

        FORCEINLINE Vector4     GetRow                  (int32 const    p_index)                        const;

        FORCEINLINE Vector3     GetScale                (float const    p_tolerance = MATH_EPSILON_F)   const;

        FORCEINLINE void        GetScaledAxes           (Vector3&       p_outXAxis,
                                                         Vector3&       p_outYAxis,
                                                         Vector3&       p_outZAxis)                     const;

        FORCEINLINE Vector3     GetScaledBackward       ()                                              const;

        FORCEINLINE Vector3     GetScaledDown           ()                                              const;

        FORCEINLINE Vector3     GetScaledForward        ()                                              const;

        FORCEINLINE Vector3     GetScaledLeft           ()                                              const;
        
        FORCEINLINE Vector3     GetScaledRight          ()                                              const;

        FORCEINLINE Vector3     GetScaledUp             ()                                              const;


        FORCEINLINE Matrix4x4   GetTransposed           ()                                              const;

        FORCEINLINE Vector3     GetUp                   ()                                              const;

        FORCEINLINE float*      GetValues               ();

        FORCEINLINE Vector3     MultiplyPoint           (Vector3 const& p_point)                        const;

        FORCEINLINE Vector3     MultiplyPoint3x4        (Vector3 const& p_point)                        const;

        FORCEINLINE Vector3     MultiplyVector          (Vector3 const& p_point)                        const;

        FORCEINLINE void        RemoveScaling           (float const    p_tolerance = MATH_EPSILON_F);

        FORCEINLINE void        SetAxis                 (int32 const    p_axisIndex,
                                                         Vector3 const& p_axis);

        FORCEINLINE void        SetAxis                 (int32 const      p_axisIndex,
                                                         float const    p_x,
                                                         float const    p_y,
                                                         float const    p_z);

        FORCEINLINE void        SetAxisX                (Vector3 const& p_axis);

        FORCEINLINE void        SetAxisX                (float const    p_x,
                                                         float const    p_y,
                                                         float const    p_z);

        FORCEINLINE void        SetAxisY                (Vector3 const& p_axis);

        FORCEINLINE void        SetAxisY                (float const    p_x,
                                                         float const    p_y,
                                                         float const    p_z);

        FORCEINLINE void        SetAxisZ                (Vector3 const& p_axis);

        FORCEINLINE void        SetAxisZ                (float const    p_x,
                                                         float const    p_y,
                                                         float const    p_z);

        FORCEINLINE void        SetColumn               (int32 const    p_index,
                                                         Vector4 const& p_column);

        FORCEINLINE void        SetIdentity             ();

        FORCEINLINE void        SetRow                  (int32 const    p_index,
                                                         Vector4 const& p_row);

        FORCEINLINE void        SetOrigin               (Vector3 const& p_newOrigin);

        FORCEINLINE void        SetOrigin               (float const    p_newX,    
                                                         float const    p_newY,
                                                         float const    p_newZ);

    //=========================[Public Local Operators]=========================//
        
        FORCEINLINE bool        operator==  (Matrix4x4 const&   p_matrix4x4)    const;
        FORCEINLINE bool        operator!=  (Matrix4x4 const&   p_matrix4x4)    const;

        FORCEINLINE Matrix4x4&  operator=   (Matrix4x4 const&   p_copy);
        FORCEINLINE Matrix4x4&  operator=   (Matrix4x4&&        p_move);

        FORCEINLINE float       operator()  (int32 const        p_rowIndex,
                                             int32 const        p_columnIndex)  const;
        FORCEINLINE float&      operator()  (int32 const        p_rowIndex,
                                             int32 const        p_columnIndex);

        FORCEINLINE float       operator()  (int32 const        p_index)        const;
        FORCEINLINE float&      operator()  (int32 const        p_index);


        FORCEINLINE Matrix4x4   operator+   (Matrix4x4 const&   p_matrix4x4)    const;
        FORCEINLINE Matrix4x4&  operator+=  (Matrix4x4 const&   p_matrix4x4);

        FORCEINLINE Matrix4x4   operator-   (Matrix4x4 const&   p_matrix4x4)    const;
        FORCEINLINE Matrix4x4&  operator-=  (Matrix4x4 const&   p_matrix4x4);

        FORCEINLINE Matrix4x4   operator*   (Matrix4x4 const&   p_matrix4x4)    const;
        FORCEINLINE Matrix4x4   operator*   (float const        p_scale)        const;
        FORCEINLINE Vector3     operator*   (Vector3 const&     p_position)     const;
        FORCEINLINE Vector4     operator*   (Vector4 const&     p_vector4)      const;

        FORCEINLINE Matrix4x4&  operator*=  (Matrix4x4 const&   p_matrix4x4);
        FORCEINLINE Matrix4x4&  operator*=  (float const        p_scale);

    private:
     //=========================[Private Local Properties]=========================//
        
        /** Array that contain all the components of this matrix */
		PROPERTY()
        std::array<float, 16>   m_values;
    
}; // !class Matrix4x4

/*==============================[Public Globale Operators]==============================*/
    
    FORCEINLINE Matrix4x4   operator*   (float const        p_scale,
                                         Matrix4x4 const&   p_matrix4x4);

#include "Matrix4x4.inl"
#include "Matrix4x4.generated.hpp"

#endif // !__MATRIX_4X4_HPP__