#version 450 core

out VertexShaderOut {
	vec2 texCoord;
} vs_out;

const vec3 PATCH_QUAD[] = vec3[] (	vec3(-0.5, 0.0, 0.5),
									vec3(0.5, 0.0, 0.5),
									vec3(0.5, 0.0, -0.5),
									vec3(-0.5, 0.0, -0.5));
									
layout(location = 21) uniform ivec2 gridSize; //patches
layout(location = 25) uniform vec2 terrainSize; //world
	
void main() {
	const vec2 PATCH_SIZE = terrainSize / vec2(gridSize);//in world units

	vec2 offset;
	
	//offset.x = mod(float(gl_InstanceID), gridSize.x) * PATCH_SIZE.x;	
	offset.x = float(gl_InstanceID & (gridSize.x - 1)) * PATCH_SIZE.x; //assuming grid size power of 2
	
	//offset.y = floor(float(gl_InstanceID) / gridSize.x) * PATCH_SIZE.y;	
	offset.y = float(int(float(gl_InstanceID) / float(gridSize.x))) * PATCH_SIZE.y;
	
	//displacement and normals textures
	vs_out.texCoord = (((PATCH_QUAD[gl_VertexID].xz + vec2(0.5)) * PATCH_SIZE) + offset) / terrainSize;
	vs_out.texCoord.t = 1.0 - vs_out.texCoord.t;
	
	vec3 instancePosition = (PATCH_QUAD[gl_VertexID] * vec3(PATCH_SIZE.x, 1, PATCH_SIZE.y)) + 
							vec3(offset.x, 0, offset.y);
	//center terrain on xz 0,0
	instancePosition.xz -= (terrainSize / 2.0) - (PATCH_SIZE / 2.0);
	
	gl_Position = vec4(instancePosition, 1.0);
}