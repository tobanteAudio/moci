#shader vertex

attribute vec3 position;

uniform mat4 u_MVP;

void main() { gl_Position = u_MVP * vec4(position, 1.0); }

#shader fragment

#ifdef GL_ES
precision mediump float;
#elif
layout(location = 0) out vec4 gl_FragColor;
#endif

uniform vec4 u_Color;

void main() { gl_FragColor = u_Color; }
