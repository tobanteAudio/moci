#shader vertex

attribute vec3 position;
attribute vec4 color;

varying vec4 v_Color;

void main()
{
    v_Color     = color;
    gl_Position = vec4(position, 1.0);
}

#shader fragment

#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_Color;

void main() { gl_FragColor = v_Color; }
