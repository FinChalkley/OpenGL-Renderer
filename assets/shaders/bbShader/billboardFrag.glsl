#version 460 core

layout(location = 0) out vec4 colour;



uniform sampler2D u_billboardImage;

in vec2 uv;


void main()
{
	

	vec4 sampledCol = texture(u_billboardImage, uv);
	float alphaMask = step(0.1, sampledCol.a);
	
	if (alphaMask == 0.0) { discard; }

	colour = sampledCol;
	//colour = vec4(1.0,0.0,0.0,1.0);

}
