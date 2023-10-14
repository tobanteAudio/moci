// Basic Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in vec2 a_TexCoords;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_Normal;
out vec3 v_FragPos;
out vec4 v_Color;
out vec2 v_TexCoords;

void main()
{
    // Model transform should already be applied to the vertex on the host side.
    mat4 model = mat4(1.0);

    v_Normal    = mat3(transpose(inverse(model))) * a_Normal;
    v_Color     = a_Color;
    v_TexCoords = a_TexCoords;

    v_FragPos   = a_Position;
    gl_Position = u_Projection * u_View * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec3 v_FragPos;
in vec4 v_Color;
in vec2 v_TexCoords;

uniform float u_Ambient;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;
uniform sampler2D u_Texture;
uniform vec3 u_LightColor;

void main()
{  // lighting
    vec3 lightColor = u_LightColor;

    // diffuse lighting
    vec3 norm     = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPos - v_FragPos);
    float diff    = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = diff * lightColor;

    // specular lighting
    float specularStrength = 0.5;

    vec3 viewDir    = normalize(u_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular   = specularStrength * spec * lightColor;

    // ambient light
    vec3 ambient = u_Ambient * lightColor;
    vec3 light   = ambient + diffuse + specular;

    // texture
    vec4 texture = texture2D(u_Texture, v_TexCoords);
    color        = vec4(light, 1.0) * texture;
}
