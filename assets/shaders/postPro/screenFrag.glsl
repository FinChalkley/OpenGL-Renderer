#version 460 core

layout(location = 0) out vec4 ppColour;

in vec2 texCoords;

uniform sampler2D u_inputTexture;

vec3 aces(vec3 x) {
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}


vec3 uncharted(vec3 x)
{
    float a = 0.15f;
    float b = 0.50f;
    float c = 0.10f;
    float d = 0.20f;
    float e = 0.02f;
    float f = 0.30f;
    return((x * (a * x + c * b) + d * e) / (x * (a * x + b) + d * f)) - e / f;
}
void main()
{
    vec3 rgb = texture(u_inputTexture, texCoords).rgb;
    rgb = aces(rgb);

    ppColour = vec4(rgb, 1.0);
}

