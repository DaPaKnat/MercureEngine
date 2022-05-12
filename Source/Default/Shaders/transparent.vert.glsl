#version 450

layout (constant_id = 0) const bool HAS_ALBEDO_MAP    = false;
layout (constant_id = 1) const bool HAS_NORMAL_MAP    = false;
layout (constant_id = 2) const bool HAS_METALLIC_MAP  = false;
layout (constant_id = 3) const bool HAS_ROUGHNESS_MAP = false;
layout (constant_id = 4) const bool HAS_AO_MAP        = false;

layout (location = 0) in vec3   in_Position;
layout (location = 1) in vec3   in_Normal;
layout (location = 2) in vec2   in_UV;
layout (location = 3) in vec3   in_Tangent;

layout (location = 0) out vec2  out_UV;

layout (set = 0, binding = 0) uniform CameraUBO
{
    vec4 position;
    mat4 view;
    mat4 projection;

} Camera;

layout (push_constant) uniform PushConstant
{
    layout (offset = 0) mat4 TRS;

} Model;

void main()
{
	gl_Position = Camera.projection * Camera.view * Model.TRS * vec4(in_Position, 1.0);

	out_UV = in_UV;
}