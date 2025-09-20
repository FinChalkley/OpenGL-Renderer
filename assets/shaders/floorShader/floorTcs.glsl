#version 460 core

layout (vertices = 3) out;

uniform float value1;
uniform float value2;


layout(std140, binding = 0) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;
};
    

float GetTessLevel(float Distance0, float Distance1){
	float AvgDistance = (Distance0 + Distance1) / 2.0;
	//calculating the tessellation level
	float TessLevel;

	TessLevel = exp((AvgDistance - value1) * value2);

    return TessLevel;


}
	
	



in vec3 vs_fragmentPos[];
in vec2 vs_texCoord[];


out vec3 tcs_fragmentPos[];
out vec2 tcs_texCoord[];


void main(){

	
	
	float eyeToVertexDist0 = distance(u_viewPos, vs_fragmentPos[0]);
	float eyeToVertexDist1 = distance(u_viewPos, vs_fragmentPos[1]);
	float eyeToVertexDist2 = distance(u_viewPos, vs_fragmentPos[2]);

	if(gl_InvocationID==0){
	//calculate the tessellation levels
		gl_TessLevelOuter[0] = GetTessLevel(eyeToVertexDist1, eyeToVertexDist2);
		gl_TessLevelOuter[1] = GetTessLevel(eyeToVertexDist2, eyeToVertexDist0);
		gl_TessLevelOuter[2] = GetTessLevel(eyeToVertexDist0, eyeToVertexDist1);
		gl_TessLevelInner[0] = gl_TessLevelOuter[2];
	}
	
	
	

	//float tessLevel = 1.0f;
	//if(gl_InvocationID==0){
	////calculate the tessellation levels
	//	gl_TessLevelOuter[0] = tessLevel;
	//	gl_TessLevelOuter[1] = tessLevel;
	//	gl_TessLevelOuter[2] = tessLevel;
	//	gl_TessLevelInner[0] = gl_TessLevelOuter[2];
	//}
	

	tcs_fragmentPos[gl_InvocationID] = vs_fragmentPos[gl_InvocationID];
	tcs_texCoord[gl_InvocationID] = vs_texCoord[gl_InvocationID];


}