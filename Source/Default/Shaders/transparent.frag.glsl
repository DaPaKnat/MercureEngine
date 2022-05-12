#version 450

layout (constant_id = 0) const bool HAS_ALBEDO_MAP    = false;
layout (constant_id = 1) const bool HAS_NORMAL_MAP    = false;
layout (constant_id = 2) const bool HAS_METALLIC_MAP  = false;
layout (constant_id = 3) const bool HAS_ROUGHNESS_MAP = false;
layout (constant_id = 4) const bool HAS_AO_MAP        = false;

layout (location = 0) in vec2   in_UV;

layout (location = 0) out vec4  out_FragColor;

layout (set = 2, binding = 0) uniform sampler2D sampler_Albedo;
layout (set = 2, binding = 1) uniform sampler2D sampler_Normal;
layout (set = 2, binding = 2) uniform sampler2D sampler_Metallic;
layout (set = 2, binding = 3) uniform sampler2D sampler_Roughness;
layout (set = 2, binding = 4) uniform sampler2D sampler_AO;

layout (push_constant) uniform PushConstant
{
    layout (offset = 64) vec4  albedo;
    layout (offset = 80) float metallic;
    layout (offset = 84) float roughness;
    layout (offset = 88) float ao;

} Material;

void main()
{
    out_FragColor = HAS_ALBEDO_MAP ? pow(texture(sampler_Albedo, in_UV), vec4(2.2)) * Material.albedo : Material.albedo;
}