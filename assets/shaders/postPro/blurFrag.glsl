#version 460 core

layout(location = 0) out vec4 ppColour;


in vec2 texCoords;

uniform sampler2D u_inputTexture;
uniform vec2 screenSize;

float blurRadius = 1.0;


void main()
{
    //size of one texel
    vec2 texelSize = 1.0/screenSize;

    vec3 result = vec3(0.0);
    float samplesTaken = 0.0;

    //3x3 kernel
    for (int x = -1; x <= 1; x++){
        for (int y = -1; y <= 1; y++){
            vec2 offset = vec2(float(x), float(y)) * texelSize * blurRadius;
            vec3 sampleCol = texture(u_inputTexture, texCoords + offset).rgb;
            result += sampleCol;
            samplesTaken++;
        }
    }

    //divide by total weight to average the colour
    result /= samplesTaken;

    ppColour = vec4(result, 1.0);

}

