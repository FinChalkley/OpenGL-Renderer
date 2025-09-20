#version 460 core
			
layout(location = 0) in vec3 a_vertexPosition;


struct Particle {
	vec4 position;
	vec4 velocity;
	float age;
};

layout(std430, binding = 0) buffer ParticleBuffer {
	Particle particles[];
};

layout (std140, binding = 0) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;
};

void main()
{
	vec4 myPos = particles[gl_VertexID].position;
	gl_Position = (u_projection * u_view) * myPos;

	gl_PointSize = 5.0;
}

