#shader vertex

attribute vec3 position;
attribute vec4 color;
attribute vec2 textureCoord;
attribute float textureIndex;
attribute float textureIsMonochrome;

uniform mat4 u_MVP;

varying vec4 v_Color;  
varying vec2 v_TexCoord;
varying float v_TexIndex;
varying float v_TexIsMonochrome;

void main() { 
    v_Color = color;
    v_TexCoord = textureCoord;
    v_TexIndex = textureIndex;
    v_TexIsMonochrome = textureIsMonochrome;
    gl_Position = u_MVP * vec4(position, 1.0);
}

#shader fragment

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D u_Textures[16];

varying vec4 v_Color;  
varying vec2 v_TexCoord;
varying float v_TexIndex;
varying float v_TexIsMonochrome;

// GLSL 100 does not support non const index into array
// int index = int(v_TexIndex);
// gl_FragColor = texture2D(u_Textures[index], v_TexCoord);
// HACK: Manual unrolling. 0-15. Depends on Texture Units max.
vec4 get_texture2D(const int index){
    if (index == 0) {
        return texture2D(u_Textures[0], v_TexCoord);
    } else if (index == 1) {
        return texture2D(u_Textures[1], v_TexCoord);
    } else if (index == 2) {
        return texture2D(u_Textures[2], v_TexCoord);
    } else if (index == 3) {
        return texture2D(u_Textures[3], v_TexCoord);
    } else if (index == 4) {
        return texture2D(u_Textures[4], v_TexCoord);
    } else if (index == 6) {
        return texture2D(u_Textures[6], v_TexCoord);
    } else if (index == 5) {
        return texture2D(u_Textures[5], v_TexCoord);
    } else if (index == 7) {
        return texture2D(u_Textures[7], v_TexCoord);
    } else if (index == 8) {
        return texture2D(u_Textures[8], v_TexCoord);
    } else if (index == 9) {
        return texture2D(u_Textures[9], v_TexCoord);
    } else if (index == 10) {
        return texture2D(u_Textures[10], v_TexCoord);
    } else if (index == 11) {
        return texture2D(u_Textures[11], v_TexCoord);
    } else if (index == 12) {
        return texture2D(u_Textures[12], v_TexCoord);
    } else if (index == 13) {
        return texture2D(u_Textures[13], v_TexCoord);
    } else if (index == 14) {
        return texture2D(u_Textures[14], v_TexCoord);
    } else if (index == 15) {
        return texture2D(u_Textures[15], v_TexCoord);
    } 
}

void main() 
{ 
    int textureIndex = int(floor(v_TexIndex));
    float _dummy = v_TexIsMonochrome;
    if(v_TexIsMonochrome == 1.0){
        float alpha = get_texture2D(textureIndex).r;
        gl_FragColor = vec4(1.0, 1.0, 1.0, alpha)* v_Color;  
    } else {
        gl_FragColor = get_texture2D(textureIndex)* v_Color;  
    }
}


          

