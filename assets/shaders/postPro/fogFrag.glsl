#version 460 core

layout(location = 0) out vec4 depthColour;

in vec2 texCoords;

uniform sampler2D u_inputTexture;
uniform sampler2D u_depthTex;
uniform vec3 u_fogColour;
uniform float u_fogDensity; //controls how thick the fog is

float near = 1.0;
float far = 1000.0;


float lineariseDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}


void main()
{
    float depthValue = texture(u_depthTex, texCoords).r;
    vec3 rgb = texture(u_inputTexture, texCoords).rgb;
    float linearDepth = lineariseDepth(depthValue);
    //float depthNormalised = (linearDepth - near) / (far - near);
    float expoDepth = 1.0 - exp(-u_fogDensity * linearDepth);
    vec3 result = mix(rgb, u_fogColour, expoDepth);
    depthColour = vec4(result, 1.0);
}
