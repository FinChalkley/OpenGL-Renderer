#version 460 core

layout(location = 0) out vec4 ppColour;

in vec2 texCoords;

uniform sampler2D u_inputTexture;

void main()
{
    vec3 rgb = texture(u_inputTexture, texCoords).rgb;
    rgb = 1.0 - rgb;
    ppColour = vec4(rgb, 1);
}

