#version 450

layout (constant_id = 0) const uint SPOT_LIGHT_COUNT        = 1u;
layout (constant_id = 1) const uint DIRECTIONAL_LIGHT_COUNT = 1u;

layout (triangles) in;

layout (triangle_strip, max_vertices = 162) out;

struct SpotLight
{
    mat4  view;
    mat4  projection;
    vec4  color;
    vec3  position;
    float intensity;
    vec3  direction;
    float angle;
};

struct DirectionalLight
{
    mat4  view;
    mat4  projection;
    vec4  color;
    vec3  direction;
    float intensity;
};

layout (set = 0, binding = 0) uniform LightUBO1
{
    uint      count;
    SpotLight lights[SPOT_LIGHT_COUNT];

} SpotLightUBO;

layout (set = 0, binding = 2) uniform LightUBO2
{
    uint             count;
    DirectionalLight lights[DIRECTIONAL_LIGHT_COUNT];

} DirectionalLightUBO;

void main()
{
    for (uint layer = 0u; layer < SpotLightUBO.count; ++layer)
    {
        for (int i = 0; i < 3; ++i)
        {
            gl_Layer    = int(layer);
            gl_Position = SpotLightUBO.lights[layer].projection * SpotLightUBO.lights[layer].view * gl_in[i].gl_Position;

            EmitVertex();
        }

        EndPrimitive();
    }

    for (uint layer = 0u; layer < DirectionalLightUBO.count; ++layer)
    {
        for (int i = 0; i < 3; ++i)
        {
            gl_Layer    = int(layer + SpotLightUBO.count);
            gl_Position = DirectionalLightUBO.lights[layer].projection * DirectionalLightUBO.lights[layer].view * gl_in[i].gl_Position;

            EmitVertex();
        }

        EndPrimitive();
    }
}