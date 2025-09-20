#version 460 core

layout(location = 0) out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D u_inputTexture;
uniform float u_expoStrength;

float exponentialStrength(vec2 texCoords, vec2 center) {
    float dist = length(texCoords - center);  
    return exp(u_expoStrength * dist) - 1.0;
}

void main()
{
    vec2 center = vec2(0.5, 0.5);
    float strength = exponentialStrength(texCoords, center);

    float redOffset   =  0.009;
    float greenOffset =  0.006;
    float blueOffset  = -0.006;

    vec2 direction = (texCoords - center) * strength;

    fragColor.r  = texture(u_inputTexture, texCoords + (direction * vec2(redOffset))).r;
    fragColor.g  = texture(u_inputTexture, texCoords + (direction * vec2(greenOffset))).g;
    fragColor.ba = texture(u_inputTexture, texCoords + (direction * vec2(blueOffset))).ba;
    fragColor = vec4(fragColor.r, fragColor.g, fragColor.ba);
}

