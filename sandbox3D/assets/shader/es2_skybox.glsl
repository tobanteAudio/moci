#shader vertex
attribute vec3 a_Position;

uniform mat4 u_Projection;
uniform mat4 u_View;

varying vec3 v_TexCoords;

void main()
{
    v_TexCoords = a_Position;
    vec4 pos    = u_Projection * u_View * vec4(a_Position, 1.0);
    gl_Position = pos.xyww;
    // gl_Position = vec4(a_Position, 1.0);
}

#shader fragment
#ifdef GL_ES
precision mediump float;
#endif

uniform samplerCube u_Skybox;

varying vec3 v_TexCoords;

void main()
{
    gl_FragColor = textureCube(u_Skybox, v_TexCoords);
    // gl_FragColor = vec4(1.0, 0.0, 1.0, 0.8);
}