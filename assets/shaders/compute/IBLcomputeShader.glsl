#version 460 core

layout(local_size_x = 16, local_size_y = 16) in;

layout(binding = 0, rgba8) uniform image2D outputCubeMap;
layout(binding = 1) uniform sampler2D equirectangularMap;


float textureWidth = textureSize(equirectangularMap, 0).x;

vec3 getSamplingVector(){

	vec2 st = gl_GlobalInvocationID.xy/vec2(textureWidth);
	vec2 uv = 2.0 * vec2(st.x, 1.0 - st.y) - vec2(1.0);
	
	vec3 sv;
	if(gl_GlobalInvocationID.z == 0) sv = vec3(1.0, uv.y, -uv.x);
	else if(gl_GlobalInvocationID.z == 1) sv = vec3(-1.0, uv.y, uv.x);
	else if(gl_GlobalInvocationID.z == 2) sv = vec3(uv.x, 1.0, -uv.y);
	else if(gl_GlobalInvocationID.z == 3) sv = vec3(uv.x, -1.0, uv.y);
	else if(gl_GlobalInvocationID.z == 4) sv = vec3(uv.x, uv.y, 1.0);
	else if(gl_GlobalInvocationID.z == 5) sv = vec3(-uv.x, uv.y, -1.0);
	return sv;
}


float PI = 3.14159265358979323846;
float TwoPI = 3.14159265358979323846 * 2.0;
void main(void) {
	
	float sampleDelta = 0.025;
	vec3 irradiance = vec3(0.0);
    int sampleCount = 0;

	vec3 v = getSamplingVector();


	//convert cartesian direction vector to spherical coordinates
	float phi = atan(v.z, v.x);
	float theta = atan(v.y);

	vec4 colour = texture(equirectangularMap, vec2(phi/TwoPI, theta/PI));
	imageStore(outputCubeMap, ivec2(gl_GlobalInvocationID), colour);

	/*
	for (float Phi = 0.0; Phi < TwoPI; Phi += sampleDelta) {
        for (float Theta = 0.0; Theta < 0.5 * PI; Theta += sampleDelta) {
            vec3 sampleDir = vec3(sin(Theta) * cos(Phi), sin(Theta) * sin(Phi), cos(Theta));
            vec2 sampleUV = vec2(atan(sampleDir.z, sampleDir.x) / TwoPI, acos(sampleDir.y) / PI);
            irradiance += texture(equirectangularMap, sampleUV).rgb * cos(Theta) * sin(Theta);
            sampleCount++;
        }
    }
	*/
	
}
