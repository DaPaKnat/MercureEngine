#include "PCH.hpp"

#include "Mathematic/Vector3.hpp"

/*==============================[Public Static Properties]==============================*/

Vector3 const   Vector3::Right              {1.0f, 0.0f, 0.0f};
Vector3 const   Vector3::Left               {-1.0f, 0.0f, 0.0f};

Vector3 const   Vector3::Up                 {0.0f, 1.0f, 0.0f};
Vector3 const   Vector3::Down               {0.0f, -1.0f, 0.0f};

Vector3 const   Vector3::Forward            {0.0f, 0.0f, 1.0f};
Vector3 const   Vector3::Backward           {0.0f, 0.0f, -1.0f};

Vector3 const   Vector3::One                {1.0f, 1.0f, 1.0f};
Vector3 const   Vector3::Zero               {0.0f, 0.0f, 0.0f};

Vector3 const   Vector3::PositiveInfinity   {MATH_INFINITY_F, MATH_INFINITY_F, MATH_INFINITY_F};
Vector3 const   Vector3::NegativeInfinity   {MATH_NINFINITY_F, MATH_NINFINITY_F, MATH_NINFINITY_F};