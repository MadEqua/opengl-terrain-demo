#version 450 core

layout (binding = 1) uniform sampler2D normalsTexture;
layout (binding = 2) uniform sampler2DArray albedoTextures;

//Shadow Sampler - Depth comparison is active on the texture
layout (binding = 3) uniform sampler2DShadow shadowMap;

layout(location = 10) uniform mat4 shadowMatrix;

layout(location = 24) uniform float displacementScale;
layout(location = 26) uniform bool enableWireframe;
layout(location = 27) uniform bool enableShadowMapping;
layout(location = 28) uniform bool enableFog;

layout(location = 30) uniform vec3 sunDirection;
layout(location = 31) uniform vec3 sunColor;

in GShaderOut {
	vec3 positionWorld;
	vec3 positionEye;
	vec4 positionShadowMap;
	vec2 texCoord;
	vec2 texCoord2;
	noperspective vec3 distToEdges;
} fs_in;

out vec4 color;

vec4 mixAlbedoTextures(float v, float t1, float t2, float t3, float transition) {
	if(v < t1 - transition) {
		return texture(albedoTextures, vec3(fs_in.texCoord2, 0.0));
	}
	else if(v < t1 + transition) {
		float factor = (v - (t1 - transition)) / (2.0 * transition);
		return mix(texture(albedoTextures, vec3(fs_in.texCoord2, 0.0)), 
					texture(albedoTextures, vec3(fs_in.texCoord2, 1.0)),
					factor);
	}
	else if(v < t2 - transition) {
		return texture(albedoTextures, vec3(fs_in.texCoord2, 1.0));
	}
	else if(v < t2 + transition) {
		float factor = (v - (t2 - transition)) / (2.0 * transition);
		return mix(texture(albedoTextures, vec3(fs_in.texCoord2, 1.0)), 
					texture(albedoTextures, vec3(fs_in.texCoord2, 2.0)),
					factor);
	}
	else if(v < t3 - transition) {
		return texture(albedoTextures, vec3(fs_in.texCoord2, 2.0));
	}
	else if(v < t3 + transition) {
		float factor = (v - (t3 - transition)) / (2.0 * transition);
		return mix(texture(albedoTextures, vec3(fs_in.texCoord2, 2.0)), 
					texture(albedoTextures, vec3(fs_in.texCoord2, 3.0)),
					factor);
	}
	else {
		return texture(albedoTextures, vec3(fs_in.texCoord2, 3.0));
	}
}

void main() {

	float shadowFactor = 1.0;
	if(enableShadowMapping) {
		//SHADOW MAPPING
		//textureProj divides each coordinate component by their last value, performing perspective division
		//the resulting 3rd component is used in the depth comparison (because its a shadow sampler with depth comparison)
		//4 texels sampled diagonally. If filtering is enabled this yields 16 texels
		float sum = 0;
		sum += textureProjOffset(shadowMap, fs_in.positionShadowMap, ivec2(-1, -1));
		sum += textureProjOffset(shadowMap, fs_in.positionShadowMap, ivec2(1, -1));
		sum += textureProjOffset(shadowMap, fs_in.positionShadowMap, ivec2(-1, 1));
		sum += textureProjOffset(shadowMap, fs_in.positionShadowMap, ivec2(1, 1));
		shadowFactor = sum * 0.25;
	}
	
	//LIGHTING
	vec3 normal = normalize((texture(normalsTexture, fs_in.texCoord).rbg * 2.0) - 1.0);
	
	vec3 ambient = sunColor * 0.15;
	vec3 diffuse = sunColor * max(0.0, dot(normal, -sunDirection));
	vec3 light = ambient + shadowFactor * diffuse;


	//MULTITEXTURING HEIGHT BASED
	float height = fs_in.positionWorld.y;

	const float THRES1 = displacementScale * 0.15;
	const float THRES2 = displacementScale * 0.45;
	const float THRES3 = displacementScale * 0.9;
	const float TRANSITION = displacementScale * 0.1;
	
	vec4 tex = mixAlbedoTextures(fs_in.positionWorld.y, THRES1, THRES2, THRES3, TRANSITION);
	
	float slope = 1.0 - normal.y;
	tex = mix(tex, 
				texture(albedoTextures, vec3(fs_in.texCoord2, 3.0)),
				slope * 0.15);
				
	color = vec4(light, 1.0) * tex;
	
	//FOG
	if(enableFog) {
		float z = length(fs_in.positionEye);

		float de = 0.0005 * smoothstep(0.0, displacementScale, displacementScale - fs_in.positionWorld.y);
		float di = 0.0002 * smoothstep(0.0, displacementScale, displacementScale - fs_in.positionWorld.y);
		
		float extintion = exp(-z * de);
		float inscattering = exp(-z * di);
		const vec4 FOG_COLOR = vec4(0.7, 0.8, 0.9, 1.0);
		color = color * extintion + FOG_COLOR * (1.0 - inscattering);
	}
	
	//WIREFRAME
	if(enableWireframe) {
		const vec4 WIREFRAME_COLOR = vec4(0.0);
		const float WIREFRAME_THICKNESS = 1.0;
		
		float wireframeStrength = 0.0;

		float minDistToEdge = min(fs_in.distToEdges.x, min(fs_in.distToEdges.y, fs_in.distToEdges.z));
		if(minDistToEdge <= WIREFRAME_THICKNESS) {
			wireframeStrength = smoothstep(0.0, 1.0, 1.0 - (minDistToEdge / WIREFRAME_THICKNESS));
		}

		color = WIREFRAME_COLOR * wireframeStrength + (1.0 - wireframeStrength) * color;
	}
}