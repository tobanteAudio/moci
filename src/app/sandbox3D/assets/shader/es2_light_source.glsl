#type vertex

attribute vec3 a_Position;
attribute vec4 a_Color;

uniform mat4 u_View;
uniform mat4 u_Projection;

varying vec4 v_Color;

void main()
{
    v_Color     = a_Color;
    gl_Position = u_Projection * u_View * vec4(a_Position, 1.0);
}

#type fragment

#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_Color;

void main() { gl_FragColor = v_Color; }
