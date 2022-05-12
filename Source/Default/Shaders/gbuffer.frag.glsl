#version 450

layout (constant_id = 0) const bool HAS_ALBEDO_MAP    = false;
layout (constant_id = 1) const bool HAS_NORMAL_MAP    = false;
layout (constant_id = 2) const bool HAS_METALLIC_MAP  = false;
layout (constant_id = 3) const bool HAS_ROUGHNESS_MAP = false;
layout (constant_id = 4) const bool HAS_AO_MAP        = false;

layout (location = 0) in vec3   in_Position;
layout (location = 1) in vec3   in_Normal;
layout (location = 2) in vec2   in_UV;
layout (location = 3) in mat3   in_TBN;

layout (location = 0) out vec4  out_FragPosition;
layout (location = 1) out vec4  out_FragNormal;
layout (location = 2) out vec4  out_FragColor;

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
	vec3 normal = HAS_NORMAL_MAP ? normalize(in_TBN * normalize(texture(sampler_Normal, in_UV).rgb * 2.0 - 1.0)) : in_Normal;
    vec3 color  = HAS_ALBEDO_MAP ? pow(texture(sampler_Albedo, in_UV).rgb, vec3(2.2)) * Material.albedo.rgb : Material.albedo.rgb;

	out_FragPosition = vec4(in_Position, HAS_METALLIC_MAP  ? texture(sampler_Metallic,  in_UV).r : Material.metallic);
    out_FragNormal   = vec4(normal,      HAS_ROUGHNESS_MAP ? texture(sampler_Roughness, in_UV).r : Material.roughness);
    out_FragColor    = vec4(color,       HAS_AO_MAP        ? texture(sampler_AO,        in_UV).r : Material.ao);
}