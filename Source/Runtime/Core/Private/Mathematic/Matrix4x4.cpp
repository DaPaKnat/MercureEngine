#include "PCH.hpp"

#include "Mathematic/Matrix4x4.hpp"

Matrix4x4 const Matrix4x4::Identity {1.0f, 0.0f, 0.0f, 0.0f,
                                     0.0f, 1.0f, 0.0f, 0.0f, 
                                     0.0f, 0.0f, 1.0f, 0.0f, 
                                     0.0f, 0.0f, 0.0f, 1.0f};

Matrix4x4 const Matrix4x4::Zero		{0.0f, 0.0f, 0.0f, 0.0f,
                                     0.0f, 0.0f, 0.0f, 0.0f, 
                                     0.0f, 0.0f, 0.0f, 0.0f, 
                                     0.0f, 0.0f, 0.0f, 0.0f};