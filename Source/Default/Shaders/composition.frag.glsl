#version 450

layout (constant_id = 0) const uint SPOT_LIGHT_COUNT        = 1u;
layout (constant_id = 1) const uint POINT_LIGHT_COUNT       = 1u;
layout (constant_id = 2) const uint DIRECTIONAL_LIGHT_COUNT = 1u;

layout (location = 0) in vec2   in_UV;

layout (location = 0) out vec4  out_FragColor;

layout (set = 0, binding = 0) uniform CameraUBO
{
    vec4 position;
    mat4 view;
    mat4 projection;

} Camera;

struct SpotLight
{
    mat4  view;
    mat4  projection;
    vec4  color;
    vec3  position;
    float intensity;
    vec3  direction;
    float cutOff;
    float outerCutOff;
};

struct PointLight
{
    mat4  views[6];
    mat4  projection;
    vec4  color;
    vec3  position;
    float intensity;
};

struct DirectionalLight
{
    mat4  view;
    mat4  projection;
    vec4  color;
    vec3  direction;
    float intensity;
};

layout (set = 1, binding = 0) uniform LightUBO1
{
    uint      count;
    SpotLight lights[SPOT_LIGHT_COUNT];

} SpotLightUBO;

layout (set = 1, binding = 1) uniform LightUBO2
{
    uint       count;
    PointLight lights[POINT_LIGHT_COUNT];

} PointLightUBO;

layout (set = 1, binding = 2) uniform LightUBO3
{
    uint             count;
    DirectionalLight lights[DIRECTIONAL_LIGHT_COUNT];

} DirectionalLightUBO;

layout (input_attachment_index = 1, set = 2, binding = 0) uniform subpassInput input_Position;
layout (input_attachment_index = 2, set = 2, binding = 1) uniform subpassInput input_Normal;
layout (input_attachment_index = 3, set = 2, binding = 2) uniform subpassInput input_Albedo;

layout (set = 2, binding = 3) uniform sampler2DArrayShadow   sampler_Shadow;
layout (set = 2, binding = 4) uniform samplerCubeArrayShadow sampler_Shadowomni;

const float PI = 3.14159265359;

float DistributionGGX(float dotNH, float roughness);
float GeometrySchlickSmithGGX(float dotNL, float dotNV, float roughness);
vec3  FresnelSchlick(float cosTheta, vec3 F0);
vec3  BRDF(vec3 L, vec3 V, vec3 N, vec3 albedo, float metallic, float roughness, vec3 F0);
float ComputeSpotShadow(vec3 position, vec3 N);
float ComputePointShadow(vec3 position, vec3 N);
float ComputeDirectionalShadow(vec3 position, vec3 N);

void main()
{	
    vec4 position = subpassLoad(input_Position);
	vec4 normal   = subpassLoad(input_Normal);
	vec4 albedo   = subpassLoad(input_Albedo);

    float metallic  = position.w;
    float roughness = normal  .w;
    float ao        = albedo  .w;

    vec3 N = normalize(normal.xyz);
    vec3 V = normalize(Camera.position.xyz - position.xyz);

    vec3 F0 = mix(vec3(0.04), albedo.rgb, metallic);

    vec3 Lo = vec3(0.0);

    // Spot Lighting.
    for (uint i = 0; i < SpotLightUBO.count; ++i)
    {
        vec3  L     = normalize(SpotLightUBO.lights[i].position - position.xyz);
        float theta = dot(L, normalize(-SpotLightUBO.lights[i].direction));

        float distance  = length(SpotLightUBO.lights[i].position - position.xyz);
        float epsilon   = SpotLightUBO.lights[i].cutOff - SpotLightUBO.lights[i].outerCutOff;
        float ratio     = clamp((theta - SpotLightUBO.lights[i].outerCutOff) / epsilon, 0.0, 1.0);
        vec3  radiance  = (SpotLightUBO.lights[i].color.rgb * SpotLightUBO.lights[i].intensity) / (distance * distance);

        Lo += BRDF(L, V, N, albedo.rgb, metallic, roughness, F0) * radiance * ratio;
    }

    // Point Lighting,
    for (uint i = 0; i < PointLightUBO.count; ++i)
    {
        vec3 L = normalize(PointLightUBO.lights[i].position - position.xyz);

        float distance = length(PointLightUBO.lights[i].position - position.xyz);
        vec3  radiance = (PointLightUBO.lights[i].color.rgb * PointLightUBO.lights[i].intensity) / (distance * distance);

        Lo += BRDF(L, V, N, albedo.rgb, metallic, roughness, F0) * radiance;
    }

    // Directional Lighting.
    for (uint i = 0; i < DirectionalLightUBO.count; ++i)
    {
        vec3 L = normalize(-DirectionalLightUBO.lights[i].direction);
        
        vec3 radiance = DirectionalLightUBO.lights[i].color.rgb * DirectionalLightUBO.lights[i].intensity;

        Lo += BRDF(L, V, N, albedo.rgb, metallic, roughness, F0) * radiance;
    }

    // Shadows.
    Lo *= ComputeSpotShadow       (position.xyz, N);
    Lo *= ComputePointShadow      (position.xyz, N);
    Lo *= ComputeDirectionalShadow(position.xyz, N);
    
    out_FragColor = vec4(albedo.rgb * 0.03 * ao + Lo, 1.0);
}

float DistributionGGX(float dotNH, float roughness)
{
    float a  = roughness * roughness;
    float a2 = a * a;
	
    float denom = (dotNH * dotNH * (a2 - 1.0) + 1.0);

    return a2 / (PI * denom * denom);
}

float GeometrySchlickSmithGGX(float dotNL, float dotNV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float GL = dotNL / (dotNL * (1.0 - k) + k);
    float GV = dotNV / (dotNV * (1.0 - k) + k);

    return GL * GV;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 BRDF(vec3 L, vec3 V, vec3 N, vec3 albedo, float metallic, float roughness, vec3 F0)
{
    vec3 H = normalize(V + L);

    float dotNH = max(dot(N, H), 0.001);
    float dotNL = max(dot(N, L), 0.001);
    float dotNV = max(dot(N, V), 0.001);
    float dotHV = max(dot(H, V), 0.001);

    float D = DistributionGGX        (dotNH, roughness);
    float G = GeometrySchlickSmithGGX(dotNL, dotNV, roughness);
    vec3  F = FresnelSchlick         (dotHV, F0);

    vec3 diffuse  = (vec3(1.0) - F) * (1.0 - metallic);
    vec3 specular = (D * G * F) / (4.0 * dotNL * dotNV);

    return (diffuse * albedo / PI + specular) * dotNL;
}

float ComputeSpotShadow(vec3 position, vec3 N)
{
    float Lo        = 1.0;
    vec2  texelSize = 1.0 / textureSize(sampler_Shadow, 0).xy;

    for (int i = 0; i < SpotLightUBO.count; ++i)
    {
        vec3  L     = normalize(-SpotLightUBO.lights[i].direction);
        float theta = dot(normalize(SpotLightUBO.lights[i].position - position.xyz), L);

        if (theta > SpotLightUBO.lights[i].outerCutOff)
        {
            vec4  lightSpacePosition = SpotLightUBO.lights[i].projection * SpotLightUBO.lights[i].view * vec4(position, 1.0);
            vec2  projCoords         = (lightSpacePosition.xy / lightSpacePosition.w) * 0.5 + 0.5;
            float depth              = (lightSpacePosition.z  / lightSpacePosition.w) - max(0.0005 * (1.0 - dot(N, L)), 0.00005);
            float shadow             = 0.0;

            for(int x = -1; x <= 1; ++x)
            {
                for(int y = -1; y <= 1; ++y)
                {
                    vec2 uv = projCoords.xy + vec2(x, y) * texelSize;

                    shadow += texture(sampler_Shadow, vec4(uv.x, uv.y, i, depth));
                }
            }

            Lo *= shadow / 9.0;
        }
    }

    return Lo;
}

float ComputePointShadow(vec3 position, vec3 N)
{
    float Lo = 1.0;

    for (int i = 0; i < PointLightUBO.count; ++i)
    {
        vec3 L = PointLightUBO.lights[i].position - position;

        float distance = (length(L) / PointLightUBO.lights[i].intensity) - max(0.0005 * (1.0 - dot(N, L)), 0.00005);
    
        Lo *= texture(sampler_Shadowomni, vec4(-L, i), distance);
    }

    return Lo;
}

float ComputeDirectionalShadow(vec3 position, vec3 N)
{
    float Lo        = 1.0;
    vec2  texelSize = 1.0 / textureSize(sampler_Shadow, 0).xy;

    for (int i = 0; i < DirectionalLightUBO.count; ++i)
    {
        vec3 L = normalize(-DirectionalLightUBO.lights[i].direction);
        
        vec4  lightSpacePosition = DirectionalLightUBO.lights[i].projection * DirectionalLightUBO.lights[i].view * vec4(position, 1.0);
        vec2  projCoords         = (lightSpacePosition.xy / lightSpacePosition.w) * 0.5 + 0.5;
        float depth              = (lightSpacePosition.z  / lightSpacePosition.w) - max(0.0005 * (1.0 - dot(N, L)), 0.00005);
        float shadow             = 0.0;

        for(int x = -1; x <= 1; ++x)
        {
            for(int y = -1; y <= 1; ++y)
            {
                vec2 uv = projCoords.xy + vec2(x, y) * texelSize;

                shadow += texture(sampler_Shadow, vec4(uv.x, uv.y, i + SpotLightUBO.count, depth));
            }
        }

        Lo *= shadow / 9.0;
    }

    return Lo;
}