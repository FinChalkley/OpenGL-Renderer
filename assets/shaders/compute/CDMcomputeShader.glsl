#version 460 core

layout(local_size_x = 16, local_size_y = 16) in;

layout(binding = 0, rgba8) uniform image2D outputImage;


uniform sampler2D u_heightMap;


void main() {
	
	
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);


	//cdm normals
	ivec2 imageSize = textureSize(u_heightMap,0);

	vec2 uv = vec2(pixel_coords) / vec2(imageSize); 
	float scale = 30.0;
	float right = (textureOffset(u_heightMap, uv, ivec2(1, 0)).r) * scale;
	float left = (textureOffset(u_heightMap, uv, ivec2(-1, 0)).r) * scale;
	float up = (textureOffset(u_heightMap, uv, ivec2(0, -1)).r) * scale;
	float down = (textureOffset(u_heightMap, uv, ivec2(0, 1)).r) * scale;

	float lr = left - right;
	float du = up - down;
	vec3 cdmNormal = normalize(vec3(lr, 2.0, du));


	float height = texture(u_heightMap, uv).r;
	//height += height * scale;


	//imageStore(outputImage, pixel_coords, vec4(p, 0.0, 0.0, 1.0));
	imageStore(outputImage, pixel_coords, vec4(cdmNormal,height));
	
}
