#version 450 core

in TCShaderOut {
	vec2 texCoord;
} tes_in[];


//how the fixed tessellator will work, and how to interpret its output
layout (quads, fractional_even_spacing, ccw) in;

layout(location = 24) uniform float displacementScale;

layout (binding = 0) uniform sampler2D displacementTexture;

layout(location = 12) uniform mat4 mvp;

void main() {
	vec2 t1 = mix(tes_in[0].texCoord, tes_in[1].texCoord, gl_TessCoord.x);
	vec2 t2 = mix(tes_in[3].texCoord, tes_in[2].texCoord, gl_TessCoord.x);
	vec2 texCoord = mix(t1, t2, gl_TessCoord.y);

	vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
	vec4 p2 = mix(gl_in[3].gl_Position, gl_in[2].gl_Position, gl_TessCoord.x);
	gl_Position = mix(p1, p2, gl_TessCoord.y);
	gl_Position.y += texture(displacementTexture, texCoord).r * displacementScale;
	gl_Position = mvp * gl_Position;
}
