#version 450

layout (location = 0) in vec2   in_Position;
layout (location = 1) in vec2   in_UV;
layout (location = 2) in vec4   in_Color;

layout (location = 0) out vec2  out_UV;
layout (location = 1) out vec4  out_Color;

layout (push_constant) uniform PushConstant
{
    vec2 scale;
    vec2 translate;

} Transform;

void main()
{
    gl_Position = vec4(in_Position * Transform.scale + Transform.translate, 0.0, 1.0);

    out_UV    = in_UV;
    out_Color = in_Color;
}