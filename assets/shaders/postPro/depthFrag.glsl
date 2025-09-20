#version 460 core

layout(location = 0) out vec4 depthColour;

in vec2 texCoords;

uniform sampler2D u_inputTexture;



float near = 1.0;
float far = 1000.0;


float lineariseDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}


void main()
{
    float depthValue = texture(u_inputTexture, texCoords).r;
    float linearDepth = lineariseDepth(depthValue);
    float depthNormalised = (linearDepth - near) / (far - near);
    depthColour = vec4(vec3(depthNormalised), 1.0);
}
