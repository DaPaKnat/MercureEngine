#version 450

layout (constant_id = 0) const bool HORIZONTAL = true;

layout (location = 0) in vec2   in_UV;

layout (location = 0) out vec4  out_FragColor;

layout (set = 0, binding = 0) uniform sampler2D sampler_Color;

const float weights[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    vec2 texelSize = 1.0 / textureSize(sampler_Color, 0);
    vec4 color     = texture(sampler_Color, in_UV);
    vec3 result    = color.rgb * weights[0];

    if (HORIZONTAL)
    {
        for (int i = 0; i < weights.length(); ++i)
        {
            result += texture(sampler_Color, in_UV + vec2(texelSize.x * i, 0.0)).rgb * weights[i];
            result += texture(sampler_Color, in_UV - vec2(texelSize.x * i, 0.0)).rgb * weights[i];
        }
    }

    else
    {
        for (int i = 0; i < weights.length(); ++i)
        {
            result += texture(sampler_Color, in_UV + vec2(0.0, texelSize.y * i)).rgb * weights[i];
            result += texture(sampler_Color, in_UV - vec2(0.0, texelSize.y * i)).rgb * weights[i];
        }
    }

    out_FragColor = vec4(result, color.a);
}