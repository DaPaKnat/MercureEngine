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

layout (location = 0) out vec3  out_Position;
layout (location = 1) out vec3  out_Normal;
layout (location = 2) out vec2  out_UV;
layout (location = 3) out mat3  out_TBN;

layout (set = 0, binding = 0) uniform UBO
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

	out_Position = (Model.TRS * vec4(in_Position, 1.0)).xyz;
	out_UV       = in_UV;

    if (HAS_NORMAL_MAP)
    {
        vec3 T = normalize(vec3(Model.TRS * vec4(in_Tangent, 0.0)));
        vec3 N = normalize(vec3(Model.TRS * vec4(in_Normal,  0.0)));

        vec3 B = cross(N, T);

        out_TBN = mat3(T, B, N);
    }

    else
	    out_Normal = mat3(transpose(inverse(Model.TRS))) * normalize(in_Normal);
}