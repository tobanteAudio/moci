#shader vertex

attribute vec4 vertex; // <vec2 pos, vec2 tex>

uniform mat4 projection;
varying vec2 TexCoords;

void main()
{
    TexCoords = vertex.zw;
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
}  

#shader fragment

#ifdef GL_ES
precision mediump float;
#endif

varying vec2 TexCoords;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(text, TexCoords).r);
    gl_FragColor = vec4(textColor, 1.0) * sampled;
}  