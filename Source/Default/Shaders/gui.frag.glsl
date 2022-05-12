#version 450

layout (location = 0) in vec2   in_UV;
layout (location = 1) in vec4   in_Color;

layout (location = 0) out vec4  out_FragColor;

layout (set = 0, binding = 0) uniform sampler2D sampler_Color;

void main()
{
    out_FragColor = in_Color * texture(sampler_Color, in_UV);
}