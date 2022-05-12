#version 450

layout (location = 0) in vec2   in_UV;

layout (location = 0) out vec4  out_FragColor;

layout (set = 0, binding = 0) uniform sampler2D sampler_Color;

const vec3 GRAYSCALE = vec3(0.2126, 0.7152, 0.0722);

void main()
{
    vec4  color      = texture(sampler_Color, in_UV);
    float brightness = dot(color.rgb, GRAYSCALE);

    if(brightness > 1.0)
        out_FragColor = color;
    else
        out_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}