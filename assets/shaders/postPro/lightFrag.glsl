#version 460 core

layout(location = 0) out vec4 colour;


struct directionalLight
{
	vec3 colour;
	vec3 direction;
};

struct pointLight
{
	vec3 colour;
	vec3 position;
	vec3 constants;
};

struct spotLight
{
	vec3 colour;
	vec3 position;
	vec3 direction;
	vec3 constants;
	float cutOff;
	float outerCutOff;
};

const int numPointLights = 50;
const int numSpotLights = 1;

layout (std140, binding = 1) uniform b_lights
{
	uniform directionalLight dLight;
	uniform pointLight pLights[numPointLights];
	uniform spotLight sLights[numSpotLights];
	uniform vec3 u_viewPos;
};





in vec2 uv;


vec3 getDirectionalLight() ;
vec3 getPointLight(int idx) ;
vec3 getSpotLight(int idx) ;


//uniform samplerCube irradinceMap;


uniform sampler2D gBufferPosWS;
uniform sampler2D gBufferNormals;
uniform sampler2D gBufferDiffSpec;
uniform sampler2D gRoughness;
uniform sampler2D gBufferDepth;
uniform sampler2D skyBox;

uniform sampler2D u_shadowMap;
uniform mat4 u_lightSpaceTransform;

//global vars
vec3 normal ;
vec3 albedoColour ;
float specularStrength  = 0.8 ;
vec3 viewDir ;
vec3 roughness ;
vec3 fragmentPos;

vec3 f0 = mix(vec3(0.04), albedoColour, specularStrength);
float PI = 3.14159265358979323846;











void main()
{
	
	
	fragmentPos = texture(gBufferPosWS, uv).rgb;
	albedoColour = texture(gBufferDiffSpec, uv).rgb;
	normal = texture(gBufferNormals, uv).rgb;
	specularStrength = texture(gBufferDiffSpec, uv).a;
	roughness = texture(gRoughness, uv).rgb;

	viewDir = normalize(u_viewPos - fragmentPos);

	vec3 result = vec3(0.0, 0.0, 0.0);
	result += getDirectionalLight();
	
	for(int i = 0; i <numPointLights; i++)
	{
		result += getPointLight(i);
	}
	
	for(int i = 0; i <numSpotLights; i++)
	{
		result += getSpotLight(i);
	}
	





	colour = vec4(result * albedoColour, 1.0) ;
	
	vec3 skyboxColour = texture(skyBox, uv).rgb;
	float depth = texture(gBufferDepth, uv).r;

	if (depth >= 0.9999f) {
		colour = vec4(skyboxColour,1.0);
	}
	
	

}

float alpha = 0.5;
vec3 freshnelSchlick(float cosTheta, vec3 f0){
	return f0 + (1.0 - f0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(float NdotH){
	float a = float(roughness) * float(roughness);
	float a2 = a * a;
	float NdotH2 = NdotH * NdotH;

	float denominator = (NdotH2 * (a2 - 1.0) + 1.0);
	denominator = PI * denominator * denominator;


	return a2/denominator;
}

float GeometrySchlickGGX(float Ndot) {
	float r = (alpha + 1.0);
	float k = (r * r) / 8.0;
	float denominator = Ndot * (1.0 - k) + k;

	return Ndot/denominator;
}

float GeometrySmith(float NdotV, float NdotL){
	float ggx1 = GeometrySchlickGGX(NdotL);
	float ggx2 = GeometrySchlickGGX(NdotV);
	return ggx1 * ggx2;
}




//getting the shadows
float ShadowCalculation() {
	float bias = 0.004;
	vec4 fragmentPosLightSpace = u_lightSpaceTransform * vec4(fragmentPos, 1.0);
	//perform perspective divide
	vec3 projCoords = fragmentPosLightSpace.xyz / fragmentPosLightSpace.w;

	vec2 texelSize = vec2(1.0 / 4096.0);

	//transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	if (projCoords.z > 1.0) {
		return 0.0;
	}

	float shadow = 0.0;
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			float closestDepth = texture(u_shadowMap, projCoords.xy + (vec2(x, y) * texelSize)).r;
			float currentDepth = projCoords.z;

			if (currentDepth  - bias > closestDepth) {
				shadow += 1.0;
			}
		}
	}

	//return (1.0);
	return (shadow / 9);
}





//lighting
vec3 getDirectionalLight()
{

	//ambient
	float ambientStrength = 0.7;
	vec3 ambient = ambientStrength * dLight.colour ;

	//diffuse
	vec3 L = normalize(-dLight.direction);
	vec3 H = normalize(L + viewDir);
	float NdotL = max(dot(normal, L), 0.0001);
	float NdotH = max(dot(normal, H), 0.0001);
	vec3 F = freshnelSchlick(NdotH,f0);
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - specularStrength;
	vec3 diffuse = (kD * albedoColour / PI) * NdotL;
	//vec3 diffuse = texture(irradinceMap, normal).rgb * albedoColour * kD;
	
	//specular
	float NdotV = max(dot(normal, viewDir), 0.0001);
	float D = DistributionGGX(NdotH);
	float G = GeometrySmith(NdotV, NdotL);

	vec3 numerator = D * G * F;
	float denominator = 4.0 * NdotV * NdotL + 0.0001;
	vec3 specular = numerator / denominator;


	//vec3 indirect = indirectDiffuse + indirectSpecular;
	//vec3 direct = Diffuse + specular;
	//luminance = indirect + direct;

	float shadowAmount = ShadowCalculation();

	
	return ambient + (1.0-shadowAmount)*(diffuse + specular);
	//return ambient + diffuse + specular;
}

vec3 getPointLight(int idx)
{
	float ambientStrength = 0.04;
	vec3 ambient = ambientStrength * pLights[idx].colour;

	float distance = length(pLights[idx].position - fragmentPos);
	float attn = 1.0/(pLights[idx].constants.x + (pLights[idx].constants.y* distance) + (pLights[idx].constants.z * (distance * distance)));
	vec3 lightDir = normalize(pLights[idx].position - fragmentPos);

	//diffuse
	vec3 L = normalize(-dLight.direction);
	vec3 H = normalize(L + viewDir);
	float NdotL = max(dot(normal, L), 0.0001);
	float NdotH = max(dot(normal, H), 0.0001);
	vec3 F = freshnelSchlick(NdotH,f0);
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - specularStrength;
	vec3 diffuse = (kD * albedoColour / PI) * NdotL;
	//vec3 diffuse = texture(irradinceMap, normal).rgb * albedoColour * kD;

	//specular
	float NdotV = max(dot(normal, viewDir), 0.0001);
	float D = DistributionGGX(NdotH);
	float G = GeometrySmith(NdotV, NdotL);

	vec3 numerator = D * G * F;
	float denominator = 4.0 * NdotV * NdotL + 0.0001;
	vec3 specular = numerator / denominator;
	
	return ambient + diffuse + specular;
}

vec3 getSpotLight(int idx)
{	
	vec3 lightDir = normalize(sLights[idx].position - fragmentPos);
	float theta = dot(lightDir, normalize(-sLights[idx].direction));
	float ambientStrength = 0.04 ;
	vec3 ambient = ambientStrength * sLights[idx].colour;

	if(theta > sLights[idx].outerCutOff)
	{

		float distance = length(sLights[idx].position - fragmentPos);
		float attn = 1.0/(sLights[idx].constants.x + (sLights[idx].constants.y* distance) + (sLights[idx].constants.z * (distance * distance)));
		//diffuse
		vec3 L = normalize(-dLight.direction);
		vec3 H = normalize(L + viewDir);
		float NdotL = max(dot(normal, L), 0.0001);
		float NdotH = max(dot(normal, H), 0.0001);
		vec3 F = freshnelSchlick(NdotH,f0);
		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - specularStrength;
		vec3 diffuse = (kD * albedoColour / PI) * NdotL;
		//vec3 diffuse = texture(irradinceMap, normal).rgb * albedoColour * kD;

		//specular
		float NdotV = max(dot(normal, viewDir), 0.0001);
		float D = DistributionGGX(NdotH);
		float G = GeometrySmith(NdotV, NdotL);

		vec3 numerator = D * G * F;
		float denominator = 4.0 * NdotV * NdotL + 0.0001;
		vec3 specular = numerator / denominator;
	
		float epsilon = sLights[idx].cutOff - sLights[idx].outerCutOff;
		float intensity = clamp((theta - sLights[idx].outerCutOff) / epsilon, 0.0, 1.0); 
	
		diffuse  *= intensity;
		specular *= intensity;
		
		return (ambient + diffuse + specular);
	}
	else
	{
		return ambient;
	}
}

