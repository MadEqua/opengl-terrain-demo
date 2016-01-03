#version 450 core

uniform samplerCube sampler;

in VertexShaderOut {
	vec3 texCoord;
} fs_in;

out vec4 color;

void main() {
	color = texture(sampler, fs_in.texCoord);
}