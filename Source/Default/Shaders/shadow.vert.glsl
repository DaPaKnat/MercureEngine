#version 450

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_UV;
layout (location = 3) in vec3 in_Tangent;

layout (push_constant) uniform PushConstant
{
    layout (offset = 0) mat4 TRS;

} Model;

void main()
{
    gl_Position = Model.TRS * vec4(in_Position, 1.0);
}