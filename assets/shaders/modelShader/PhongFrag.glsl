#version 460 core

//layout(location = 0) out vec4 colour;
layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gDiffSpec;
layout(location = 3) out vec4 gRoughness;


in vec3 fragmentPos;
in vec2 texCoord;
in mat3 TBN;


layout (std140, binding = 0) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;
};


uniform sampler2D u_albedoMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_metalMap;
uniform sampler2D u_roughMap;

//global vars
vec3 normal ;
vec3 albedoColour ;
vec3 roughColour ;
float specularStrength  = 0.8 ;
vec3 viewDir ;


void main()
{
	vec3 result = vec3(0.0, 0.0, 0.0); 
    viewDir = normalize(u_viewPos - fragmentPos);
	
	specularStrength = texture(u_metalMap, texCoord).r;
	albedoColour = texture(u_albedoMap, texCoord).rgb;
	vec3 N = texture(u_normalMap, texCoord).rgb;
	roughColour = texture(u_roughMap, texCoord).rgb;
	normal = normalize(TBN * (N * 2.0 - 1.0));

	//attachments
	gPosition = vec4(fragmentPos, 1.0);
	gNormal = vec4(normal, 0.0);
	gDiffSpec = vec4(albedoColour, specularStrength);
	gRoughness = vec4(roughColour, 1.0);
	      

}

