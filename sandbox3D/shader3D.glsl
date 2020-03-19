#shader vertex

attribute vec3 a_Position;
attribute vec3 a_Normal;
attribute vec4 a_Color;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

varying vec3 v_Normal;
varying vec3 v_FragPos;
varying vec4 v_Color;

void main()
{
    v_Normal    = a_Normal;
    v_Color     = a_Color;
    v_FragPos   = vec3(u_Model * vec4(a_Position, 1.0));
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}

#shader fragment

#ifdef GL_ES
precision mediump float;
#endif

uniform float u_Ambient;
uniform vec3 u_LightPos;

varying vec3 v_Normal;
varying vec3 v_FragPos;
varying vec4 v_Color;

void main()
{
    float const lightColor = vec3(1.0, 1.0, 1.0);

    vec3 norm     = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPos - v_FragPos);

    float diff   = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 ambient = u_Ambient * lightColor;
    vec3 light   = ambient + diffuse;

    vec4 color   = vec4(light, 1.0) * v_Color;
    gl_FragColor = color;
}
