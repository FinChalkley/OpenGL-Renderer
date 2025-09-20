#version 460 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

layout(std140, binding = 0) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;
};

mat4 VP;
out vec2 uv;

void main()
{
	VP = u_projection * u_view;

	vec3 pos = gl_in[0].gl_Position.xyz;
	vec3 toCamera = normalize(u_viewPos - pos);
	vec3 up = vec3(0.0,1.0,0.0);
	vec3 right = normalize(cross(toCamera, up));

	//float distance = length(u_viewPos - pos);
	float size = 5.f;

	//defining the corners
	vec3 corner0 = pos - right * size - up * size; //bottom left
	vec3 corner1 = pos + right * size - up * size; //bottom right
	vec3 corner2 = pos - right * size + up * size; //top left
	vec3 corner3 = pos + right * size + up * size; //top right

	//emitting vertices
	uv = vec2(0.0,0.0); //bottom left
	gl_Position = VP * vec4(corner0, 1.0);
	EmitVertex();
	
	uv = vec2(1.0,0.0); //bottom right
	gl_Position = VP * vec4(corner1, 1.0);
	EmitVertex();
	
	uv = vec2(0.0,1.0); //top left
	gl_Position = VP * vec4(corner2, 1.0);
	EmitVertex();

	uv = vec2(1.0,1.0); //top right
	gl_Position = VP * vec4(corner3, 1.0);
	EmitVertex();


	EndPrimitive();
}
