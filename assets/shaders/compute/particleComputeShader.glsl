#version 460 core

layout(local_size_x = 16, local_size_y = 16) in;

vec2 gid = gl_GlobalInvocationID.xy;
int gridWidth = 32;
uint index = uint(gid.x) * gridWidth + uint(gid.y);



struct Particle {
	vec4 position;
	vec4 velocity;
	float age;
};

layout(std430, binding = 0) buffer ParticleBuffer {
	Particle particles[];
};
//uniform float timeStep;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}
float calcAge() {
	return 10.f;
}


void main() {
	
	if(index >= particles.length()){
		return;
	}

	particles[index].position.xyz;
	particles[index].age = calcAge();
	particles[index].velocity.xyz;


}
