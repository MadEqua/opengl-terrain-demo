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

vec4 triplanarMapping(vec3 normal, sampler2DArray tex, float layer, vec3 worldPos) {
    const float SCALE = 0.05;

    normal = abs(normal);
    if(normal.x > normal.y && normal.x > normal.z) {
         return texture(tex, vec3(worldPos.yz * SCALE, layer));
    }
    else if(normal.y > normal.z && normal.y > normal.x) {
         return texture(tex, vec3(worldPos.xz * SCALE, layer));
    }
    else {
         return texture(tex, vec3(worldPos.xy * SCALE, layer));
    }
}

vec4 mixAlbedoTextures(float v, float t1, float t2, float t3, float transition, vec3 normal) {
	if(v < t1 - transition) {
        return triplanarMapping(normal, albedoTextures, 0.0, fs_in.positionWorld);
	}
	else if(v < t1 + transition) {
		float factor = (v - (t1 - transition)) / (2.0 * transition);
        vec4 t1 = triplanarMapping(normal, albedoTextures, 0.0, fs_in.positionWorld);
        vec4 t2 = triplanarMapping(normal, albedoTextures, 1.0, fs_in.positionWorld);
        return mix(t1, t2, factor);
	}
	else if(v < t2 - transition) {
        return triplanarMapping(normal, albedoTextures, 1.0, fs_in.positionWorld);
	}
	else if(v < t2 + transition) {
		float factor = (v - (t2 - transition)) / (2.0 * transition);
        vec4 t1 = triplanarMapping(normal, albedoTextures, 1.0, fs_in.positionWorld);
        vec4 t2 = triplanarMapping(normal, albedoTextures, 2.0, fs_in.positionWorld);
        return mix(t1, t2, factor);
	}
	else if(v < t3 - transition) {
        return triplanarMapping(normal, albedoTextures, 2.0, fs_in.positionWorld);
	}
	else if(v < t3 + transition) {
		float factor = (v - (t3 - transition)) / (2.0 * transition);
        vec4 t1 = triplanarMapping(normal, albedoTextures, 2.0, fs_in.positionWorld);
        vec4 t2 = triplanarMapping(normal, albedoTextures, 3.0, fs_in.positionWorld);
        return mix(t1, t2, factor);
	}
	else {
        return triplanarMapping(normal, albedoTextures, 3.0, fs_in.positionWorld);
	}
}

void main() {

	float shadowFactor = 1.0;
	if(enableShadowMapping) {
		//SHADOW MAPPING
		//textureProj divides each coordinate component by their last value, performing perspective division
		//the resulting 3rd component is used in the depth comparison (because its a shadow sampler with depth comparison)
		//4 texels sampled diagonally. If filtering is enabled this yields 16 texels
		float sum = 0.0;
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


	//MULTITEXTURING SLOPE BASED
	const float THRES1 = 0.03;
	const float THRES2 = 0.4;
	const float THRES3 = 0.7;
	const float TRANSITION = 0.15;
	
	vec4 tex = mixAlbedoTextures(1.0 - normal.y, THRES1, THRES2, THRES3, TRANSITION, normal);
	color = tex * vec4(light, 1.0);

    //FOG
	if(enableFog) {
		float z = length(fs_in.positionEye);

		float de = 0.0009 * smoothstep(0.0, displacementScale, displacementScale - fs_in.positionWorld.y);
		float di = 0.0009 * smoothstep(0.0, displacementScale, displacementScale - fs_in.positionWorld.y);
		
		float extintion = exp(-z * de);
		float inscattering = exp(-z * di);
		const vec4 FOG_COLOR = vec4(0.7, 0.8, 0.9, 1.0);
		color = color * extintion + FOG_COLOR * (1.0 - inscattering);
	}
	
	//WIREFRAME
	if(enableWireframe) {
		const vec4 WIREFRAME_COLOR = vec4(0);
		const float WIREFRAME_THICKNESS = 1.0;
		
		float wireframeStrength = 0;

		float minDistToEdge = min(fs_in.distToEdges.x, min(fs_in.distToEdges.y, fs_in.distToEdges.z));
		if(minDistToEdge <= WIREFRAME_THICKNESS) {
			wireframeStrength = smoothstep(0, 1, 1.0 - (minDistToEdge / WIREFRAME_THICKNESS));
		}

		color = WIREFRAME_COLOR * wireframeStrength + (1 - wireframeStrength) * color;
	}
}