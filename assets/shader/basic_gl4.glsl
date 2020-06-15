#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 textureCoord;

out vec4 v_Color;
out vec2 v_TexCoord;

void main()
{
    v_Color     = color;
    v_TexCoord  = textureCoord;
    gl_Position = vec4(position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_Textures[16];

in vec4 v_Color;
in vec2 v_TexCoord;

void main()
{
    int index    = int(0);
    vec4 texture = texture2D(u_Textures[index], v_TexCoord);
    color        = texture;
}
