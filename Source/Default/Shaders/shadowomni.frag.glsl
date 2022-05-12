#version 450

layout (location = 0) in float in_Distance;

void main()
{
    gl_FragDepth = in_Distance;
}