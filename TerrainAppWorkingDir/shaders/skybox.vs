#version 450 core

layout (location = 0) in vec3 position;

layout(location = 10) uniform mat4 mvp;

out VertexShaderOut {
	vec3 texCoord;
} vs_out;


void main() {
	vs_out.texCoord = vec3(position.x, -position.y, position.z);
	gl_Position = mvp * vec4(position, 1.0);
}