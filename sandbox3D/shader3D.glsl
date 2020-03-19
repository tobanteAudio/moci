#shader vertex

attribute vec3 position;
attribute vec3 normal;
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

uniform float u_Ambient;

varying vec4 v_Color;  


void main() 
{ 
    vec3 ambient = u_Ambient * vec3(1.0, 1.0, 1.0);
    vec4 color = vec4(ambient, 1.0) * v_Color;
    gl_FragColor = color;  
}


          

