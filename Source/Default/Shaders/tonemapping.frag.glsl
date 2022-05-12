#version 450

layout (location = 0) in vec2   in_UV;

layout (location = 0) out vec4  out_FragColor;

layout (set = 0, binding = 0) uniform sampler2D sampler_HDR;
layout (set = 0, binding = 1) uniform sampler2D sampler_Bloom;

void main()
{
    vec4 color = texture(sampler_HDR, in_UV) + texture(sampler_Bloom, in_UV);

    out_FragColor = vec4(pow(color.rgb / (color.rgb + vec3(1.0)), vec3(1.0 / 2.2)), color.a);
}