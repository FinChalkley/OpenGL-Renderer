#version 460 core

layout(local_size_x = 16, local_size_y = 16) in;

vec2 gid = gl_GlobalInvocationID.xy;
int gridWidth = 32 * 16;
uint index = uint(gid.x) + gridWidth * uint(gid.y);



struct Particle {
	vec4 position;
	vec4 velocity;
	float age;
};

layout(std430, binding = 0) buffer ParticleBuffer {
	Particle particles[];
};

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}
float calcAge(vec2 seed) {
	return mix(3.0, 10.0, rand(seed));
}
vec3 calcPos(){
	vec2 randOffset = vec2(rand(gid.xy * 12.5), rand(gid.yx * 42.3));
	vec3 newPos = vec3(randOffset.x * 60.0 - 30.0, 15.0, randOffset.y * 60.0 - 30.0);
	return newPos;
}

uniform float timeStep;

void main() {
	

	if(index >= particles.length()){
		return;
	}
	
	particles[index].age = particles[index].age - timeStep;



	if (particles[index].age <= 0.0) {

		particles[index].position.xyz = calcPos();
		particles[index].age = calcAge(vec2(index, index * 1.234));
		particles[index].position.xyz += particles[index].velocity.xyz * timeStep * 2.0;
	}
	else {
		particles[index].position.xyz += particles[index].velocity.xyz * timeStep * 2.0;
	}
}
