#version 450 core

layout (triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in TEShaderOut {
	vec2 texCoord;
	vec2 texCoord2;
} gs_in[];

out GShaderOut {
	vec3 positionWorld;
	vec3 positionEye;
	vec4 positionShadowMap;
	vec2 texCoord;
	vec2 texCoord2;
	noperspective vec3 distToEdges;
} gs_out;

layout(location = 11) uniform mat4 view;
layout(location = 13) uniform mat4 projection;
layout(location = 10) uniform mat4 shadowMatrix; //MVP B = bias, remapping [-1, 1] to [0, 1]

layout(location = 23) uniform ivec2 viewportSize;

mat4 pvMatrix = projection * view;

vec2 projectToViewportSpace(vec4 point) {
	vec4 clip = pvMatrix * point;
	clip.xy /= clip.w;
	clip.xy = ((clip.xy * 0.5) + 0.5) * viewportSize;
	return clip.xy;
}

void emitVertex(int i, vec3 distToEdges) {
	gs_out.positionWorld = gl_in[i].gl_Position.xyz;
	gs_out.positionEye = (view * gl_in[i].gl_Position).xyz;
	gs_out.positionShadowMap = shadowMatrix * gl_in[i].gl_Position;
	
	gs_out.texCoord = gs_in[i].texCoord;
	gs_out.texCoord2 = gs_in[i].texCoord2;
	
	gs_out.distToEdges = distToEdges;

	gl_Position = pvMatrix * gl_in[i].gl_Position;
	
	EmitVertex();
}

void main() {

	vec2 p0 = projectToViewportSpace(gl_in[0].gl_Position);
	vec2 p1 = projectToViewportSpace(gl_in[1].gl_Position);
	vec2 p2 = projectToViewportSpace(gl_in[2].gl_Position);

	vec2 e0 = p1 - p0;
	vec2 e1 = p2 - p1;
	vec2 e2 = p2 - p0;

	//Triangle area = 1/2 * abs(cross(u, v)) with u and v being vectors from the same vertex
	float doubleTriArea = abs((e0.x * e2.y) - (e0.y * e2.x)); 

	//Triangle area = Base * Height / 2 => Height = 2 * Area / Base
	float h0 = doubleTriArea / length(e0);
	float h1 = doubleTriArea / length(e1);
	float h2 = doubleTriArea / length(e2);

	emitVertex(0, vec3(0, h1, 0));
	emitVertex(1, vec3(0, 0, h2));
	emitVertex(2, vec3(h0, 0, 0));
	EndPrimitive();
}