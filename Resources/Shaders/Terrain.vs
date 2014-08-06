uniform mat4 u_mvp;

uniform sampler2D u_texture0;	//height map
uniform mat4 u_lightmvp;
uniform mat4 u_maTexSpace;

attribute vec3 a_posL;
attribute vec2 a_uv;

varying vec2 v_uv;
varying vec4 v_terrainLightTexSpace;


void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position = u_mvp*posL;

	v_uv = a_uv;

	//calculate terrain position in ligh space
	//...
	//calculate terrain postion in texture space (convert position in [-1,1] limit to [0,1] limit - limit of texture space)
	//...
}