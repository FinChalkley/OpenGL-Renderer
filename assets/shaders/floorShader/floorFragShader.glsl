#version 460 core

//layout(location = 0) out vec4 colour;
layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gDiffSpec;
layout(location = 3) out vec4 gRoughness;


in vec3 vertexNormal;
in vec3 fragmentPos;
in vec2 texCoord;
in float scale;


layout (std140, binding = 0) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;
};


uniform sampler2D u_albedoMap;
uniform sampler2D u_GrassMetalMap;
uniform sampler2D u_GrassRoughMap;

uniform sampler2D u_rockTex;

uniform sampler2D u_RockMetalMap;

uniform sampler2D u_RockRoughMap;



//global vars
vec3 GrassColour ;
vec3 GrassMetalColour ;
vec3 GrassRoughColour ;

vec3 rockCol ;
vec3 RockMetalColour ;
vec3 RockRoughColour ;

vec3 normal ;
float specularStrength  = 0.8 ;
vec3 viewDir ;
vec3 col;
vec3 metalCol;
vec3 roughCol;


void main()
{
	vec3 result = vec3(0.0, 0.0, 0.0); 
    viewDir = normalize(u_viewPos - fragmentPos);
	normal = normalize(vertexNormal) ;

	GrassColour = texture(u_albedoMap, texCoord).rgb;
	GrassMetalColour = texture(u_GrassMetalMap, texCoord).rgb;
	GrassRoughColour = texture(u_GrassRoughMap, texCoord).rgb;

	rockCol = texture(u_rockTex, texCoord).rgb;
	RockMetalColour = texture(u_RockMetalMap, texCoord).rgb;
	RockRoughColour = texture(u_RockRoughMap, texCoord).rgb;

	float mid = scale * 0.5;
	//vec3 col1 = vec3(0.2, 0.6, 0.1);
	//vec3 col2 = vec3(0.6, 0.2, 0.1);
	col = mix(GrassColour, rockCol, smoothstep(0.0, mid + mid * 0.5, fragmentPos.y + 5.0));
	roughCol = mix(GrassRoughColour, rockCol, smoothstep(0.0, mid + mid * 0.5, fragmentPos.y + 5.0));
	metalCol = mix(GrassMetalColour, rockCol, smoothstep(0.0, mid + mid * 0.5, fragmentPos.y + 5.0));

	specularStrength = vec3(metalCol).r;
	//attachments
	gPosition = vec4(fragmentPos, 1.0);
	gNormal = vec4(normal, 0.0);
	gDiffSpec = vec4(col, specularStrength);
	gRoughness = vec4(roughCol, 1.0);
	

}


