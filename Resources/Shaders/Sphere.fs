precision mediump float;

uniform sampler2D u_texture0;	//Rock
uniform sampler2D u_texture1;	//light

varying vec2 v_uv;

void main()
{
	gl_FragColor = texture2D(u_texture1, v_uv);
}
