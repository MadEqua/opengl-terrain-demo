#version 450 core

//how the fixed tessellator will work, and how to interpret its output
layout (quads, fractional_even_spacing, ccw) in;

layout(location = 24) uniform float displacementScale;

layout (binding = 0) uniform sampler2D displacementTexture;

layout(location = 11) uniform mat4 view;

in TCShaderOut {
	vec2 texCoord;
	vec2 texCoord2;
} tes_in[];

out TEShaderOut {
	vec2 texCoord;
	vec2 texCoord2;
} tes_out;

void main() {
	vec2 t1 = mix(tes_in[0].texCoord, tes_in[1].texCoord, gl_TessCoord.x);
	vec2 t2 = mix(tes_in[3].texCoord, tes_in[2].texCoord, gl_TessCoord.x);
	tes_out.texCoord = mix(t1, t2, gl_TessCoord.y);
	
	vec2 t21 = mix(tes_in[0].texCoord2, tes_in[1].texCoord2, gl_TessCoord.x);
	vec2 t22 = mix(tes_in[3].texCoord2, tes_in[2].texCoord2, gl_TessCoord.x);
	tes_out.texCoord2 = mix(t21, t22, gl_TessCoord.y);
	
	vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
	vec4 p2 = mix(gl_in[3].gl_Position, gl_in[2].gl_Position, gl_TessCoord.x);
	gl_Position = mix(p1, p2, gl_TessCoord.y);
	gl_Position.y += texture(displacementTexture, tes_out.texCoord).r * displacementScale;
}
