#include "PCH.hpp"

#include "Mathematic/Vector2.hpp"

/*==============================[Public Static Properties]==============================*/

Vector2 const   Vector2::Right              {1.0f, 0.0f};
Vector2 const   Vector2::Left               {-1.0f, 0.0f};

Vector2 const   Vector2::Up                 {0.0f, 1.0f};
Vector2 const   Vector2::Down               {0.0f, -1.0f};

Vector2 const   Vector2::One                {1.0f, 1.0f};
Vector2 const   Vector2::Zero               {0.0f, 0.0f};

Vector2 const   Vector2::PositiveInfinity   {MATH_INFINITY_F, MATH_INFINITY_F};
Vector2 const   Vector2::NegativeInfinity   {MATH_NINFINITY_F, MATH_NINFINITY_F};