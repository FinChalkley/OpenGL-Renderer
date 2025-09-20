#version 460 core
layout(local_size_x = 16, local_size_y = 16) in;

layout(binding = 0, rgba8) uniform image2D outputImage;

//uniform sampler2D u_image;
uniform sampler2D u_heightMap;
uniform float u_octaves;
uniform float u_amplitude;
uniform float u_frequency;
uniform float u_persitance;
uniform float u_lacunarity;


float remap(float currValue, float  inMin,float inMax,float outMin, float outMax);
vec2 hash2(vec2 p) ;
float noise(in vec2 p);
float FBM(vec2 position);



float remap(float currValue, float  inMin,float inMax,float outMin, float outMax) {
	
	float t = (currValue - inMin) / (inMax - inMin);
	return mix(outMin, outMax, t);
}
vec2 hash2(vec2 p) 
{
	p = vec2(dot(p, vec2(127.1, 311.7)),
		dot(p, vec2(269.5, 183.3)));

	return -1.0 + 2.0 * fract(sin(p) * 43758.5453123 );
}

float noise(in vec2 p)
{
	vec2 i = floor(p);
	vec2 f = fract(p);

	vec2 u = f * f * (3.0 - 2.0 * f);

	return mix(mix(dot(hash2(i + vec2(0.0, 0.0)), f - vec2(0.0, 0.0)),
		dot(hash2(i + vec2(1.0, 0.0)), f - vec2(1.0, 0.0)), u.x),
		mix(dot(hash2(i + vec2(0.0, 1.0)), f - vec2(0.0, 1.0)),
			dot(hash2(i + vec2(1.0, 1.0)), f - vec2(1.0, 1.0)), u.x), u.y);
}

float FBM(vec2 position)
{
    float total = 0.0;
    float amplitude = u_amplitude;
    float frequency = u_frequency;
    float maxAmplitude = 0.0;

    for (int i = 0; i < u_octaves; i++)
    {
        total += noise(position * frequency) * amplitude;
        frequency = u_lacunarity;
        amplitude = u_persitance;
        maxAmplitude += amplitude;
    }

    return total / maxAmplitude;
}



void main() {
	
	

	ivec2 imageSize = textureSize(u_heightMap,0);
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	vec2 uv = vec2(pixel_coords) / vec2(imageSize); 

	//making noise maps
	float p = FBM(uv);
	
	p = remap(p, -1.0, 1.0, 0.0, 1.0);

	

	imageStore(outputImage, pixel_coords, vec4(vec3(p), 1.0));
}


