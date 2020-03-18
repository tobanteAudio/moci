#shader vertex

attribute vec3 position;
attribute vec4 color;

uniform mat4 u_MVP;

varying vec4 v_Color;  

void main() { 
    v_Color = color;
    gl_Position = u_MVP * vec4(position, 1.0);
}

#shader fragment

#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_Color;  

void main() 
{ 
    gl_FragColor = v_Color;  
}


          

