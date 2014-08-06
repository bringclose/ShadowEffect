precision mediump float;

uniform sampler2D u_texture1;	//blend map
uniform sampler2D u_texture2;	//Grass
uniform sampler2D u_texture3;	//Rock
uniform sampler2D u_texture4;	//Dirt
uniform sampler2D u_shadowTexture;	//shadow map

varying vec2 v_uv;
varying vec4 v_terrainLightTexSpace;

#define shadowColor vec4(0.5, 0.5, 0.5, 1.0)

void main()
{
	//calculate color for terrain
	float scaledFactor = 30.0;
	vec2 scaled_uv = v_uv * scaledFactor;
	vec4 blendMap = texture2D(u_texture1, v_uv);
	vec4 grass = texture2D(u_texture2, scaled_uv);
	vec4 rock = texture2D(u_texture3, scaled_uv);
	vec4 dirt = texture2D(u_texture4, scaled_uv);
	vec4 terrain = (blendMap.r * grass + blendMap.g * rock + blendMap.b * dirt) / (blendMap.r + blendMap.g + blendMap.b);

	//calculate depth value in Light space
	//...

	//calculate depth value in Shadow Map
	//...

	//apply shadow color on terrain base on depth comparation
	//...

	gl_FragColor = terrain;
}