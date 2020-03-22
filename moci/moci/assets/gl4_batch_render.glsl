#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 textureCoord;
layout(location = 3) in float textureIndex;
layout(location = 4) in float textureIsMonochrome;

uniform mat4 u_MVP;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out float v_TexIsMonochrome;

void main()
{
    v_Color           = color;
    v_TexCoord        = textureCoord;
    v_TexIndex        = textureIndex;
    v_TexIsMonochrome = textureIsMonochrome;
    gl_Position       = u_MVP * vec4(position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_Textures[16];

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TexIsMonochrome;

void main()
{
    int index    = int(v_TexIndex);
    vec4 texture = texture2D(u_Textures[index], v_TexCoord);

    if (v_TexIsMonochrome == 1.0)
    {
        float alpha = texture.r;
        color       = vec4(1.0, 1.0, 1.0, alpha) * v_Color;
    }
    else
    {
        color = texture * v_Color;
    }
}
