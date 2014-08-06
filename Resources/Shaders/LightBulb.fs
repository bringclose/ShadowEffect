precision mediump float;

uniform sampler2D u_texture0;	//light bulb

varying vec2 v_uv; 

void main() 
{ 	
	gl_FragColor = texture2D(u_texture0, v_uv);
}
