#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 tes_fragmentPos[];
in vec3 tes_normal[];
in vec2 tes_texCoord[];
in float tes_scale[];




out vec3 vertexNormal;
out vec3 fragmentPos;
out vec2 texCoord;
out float scale;

vec3 getNormal() {
	vec3 a = tes_fragmentPos[0] - tes_fragmentPos[1];
	vec3 b = tes_fragmentPos[0] - tes_fragmentPos[2];
	return normalize(cross(a,b));
}


void main()
{
	scale = tes_scale[0];

	for(int i = 0; i < 3; i++){
		texCoord = tes_texCoord[i];
		fragmentPos = tes_fragmentPos[i];
		//vertexNormal = getNormal();
		vertexNormal = tes_normal[i];
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();

	}

	EndPrimitive();
}


