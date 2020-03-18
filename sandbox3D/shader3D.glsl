#shader vertex

attribute vec3 position;

uniform mat4 u_MVP;

varying vec4 v_Color;  

void main() { 
    v_Color = vec4(1.0,1.0,0.0,1.0);
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


          

