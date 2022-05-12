#version 450

layout (constant_id = 0) const uint POINT_LIGHT_COUNT = 1u;

layout (triangles) in;

layout (triangle_strip, max_vertices = 162) out;

layout (location = 0) out float out_Distance;

struct PointLight
{
    mat4  views[6];
    mat4  projection;
    vec4  color;
    vec3  position;
    float intensity;
};

layout (set = 0, binding = 1) uniform LightUBO
{
    uint       count;
    PointLight lights[POINT_LIGHT_COUNT];

} PointLightUBO;

void main()
{
    for (uint layer = 0u; layer < PointLightUBO.count; ++layer)
    {
        for (uint face = 0u; face < 6u; ++face)
        {
            for (int i = 0; i < 3; ++i)
            {
                out_Distance = length(PointLightUBO.lights[layer].position - gl_in[i].gl_Position.xyz) / PointLightUBO.lights[layer].intensity;

                gl_Layer    = int(6u * layer + face);
                gl_Position = PointLightUBO.lights[layer].projection * PointLightUBO.lights[layer].views[face] * gl_in[i].gl_Position;

                EmitVertex();
            }

            EndPrimitive();
        }
    }
}