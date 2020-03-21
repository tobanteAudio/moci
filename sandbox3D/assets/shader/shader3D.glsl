#shader vertex

attribute vec3 a_Position;
attribute vec3 a_Normal;
attribute vec4 a_Color;

uniform mat4 u_View;
uniform mat4 u_Projection;

varying vec3 v_Normal;
varying vec3 v_FragPos;
varying vec4 v_Color;

float Inverse(float m) { return 1.0 / m; }

mat2 Inverse(mat2 m) { return mat2(m[1][1], -m[0][1], -m[1][0], m[0][0]) / (m[0][0] * m[1][1] - m[0][1] * m[1][0]); }

mat3 Inverse(mat3 m)
{
    float a00 = m[0][0], a01 = m[0][1], a02 = m[0][2];
    float a10 = m[1][0], a11 = m[1][1], a12 = m[1][2];
    float a20 = m[2][0], a21 = m[2][1], a22 = m[2][2];

    float b01 = a22 * a11 - a12 * a21;
    float b11 = -a22 * a10 + a12 * a20;
    float b21 = a21 * a10 - a11 * a20;

    float det = a00 * b01 + a01 * b11 + a02 * b21;

    return mat3(b01, (-a22 * a01 + a02 * a21), (a12 * a01 - a02 * a11), b11, (a22 * a00 - a02 * a20),
                (-a12 * a00 + a02 * a10), b21, (-a21 * a00 + a01 * a20), (a11 * a00 - a01 * a10))
           / det;
}

mat4 Inverse(mat4 m)
{
    float a00 = m[0][0], a01 = m[0][1], a02 = m[0][2], a03 = m[0][3],  //
        a10 = m[1][0], a11 = m[1][1], a12 = m[1][2], a13 = m[1][3],    //
        a20 = m[2][0], a21 = m[2][1], a22 = m[2][2], a23 = m[2][3],    //
        a30 = m[3][0], a31 = m[3][1], a32 = m[3][2], a33 = m[3][3],    //

        b00 = a00 * a11 - a01 * a10,  //
        b01 = a00 * a12 - a02 * a10,  //
        b02 = a00 * a13 - a03 * a10,  //
        b03 = a01 * a12 - a02 * a11,  //
        b04 = a01 * a13 - a03 * a11,  //
        b05 = a02 * a13 - a03 * a12,  //
        b06 = a20 * a31 - a21 * a30,  //
        b07 = a20 * a32 - a22 * a30,  //
        b08 = a20 * a33 - a23 * a30,  //
        b09 = a21 * a32 - a22 * a31,  //
        b10 = a21 * a33 - a23 * a31,  //
        b11 = a22 * a33 - a23 * a32,  //

        det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

    return mat4(                                   //
               a11 * b11 - a12 * b10 + a13 * b09,  //
               a02 * b10 - a01 * b11 - a03 * b09,  //
               a31 * b05 - a32 * b04 + a33 * b03,  //
               a22 * b04 - a21 * b05 - a23 * b03,  //
               a12 * b08 - a10 * b11 - a13 * b07,  //
               a00 * b11 - a02 * b08 + a03 * b07,  //
               a32 * b02 - a30 * b05 - a33 * b01,  //
               a20 * b05 - a22 * b02 + a23 * b01,  //
               a10 * b10 - a11 * b08 + a13 * b06,  //
               a01 * b08 - a00 * b10 - a03 * b06,  //
               a30 * b04 - a31 * b02 + a33 * b00,  //
               a21 * b02 - a20 * b04 - a23 * b00,  //
               a11 * b07 - a10 * b09 - a12 * b06,  //
               a00 * b09 - a01 * b07 + a02 * b06,  //
               a31 * b01 - a30 * b03 - a32 * b00,  //
               a20 * b03 - a21 * b01 + a22 * b00   //
               )
           / det;
}

float Transpose(float m) { return m; }

mat2 Transpose(mat2 m)
{
    return mat2(m[0][0], m[1][0],  //
                m[0][1], m[1][1]   //
    );
}

mat3 Transpose(mat3 m)
{
    return mat3(m[0][0], m[1][0], m[2][0],  //
                m[0][1], m[1][1], m[2][1],  //
                m[0][2], m[1][2], m[2][2]   //
    );
}

mat4 Transpose(mat4 m)
{
    return mat4(m[0][0], m[1][0], m[2][0], m[3][0],  //
                m[0][1], m[1][1], m[2][1], m[3][1],  //
                m[0][2], m[1][2], m[2][2], m[3][2],  //
                m[0][3], m[1][3], m[2][3], m[3][3]   //
    );
}

void main()
{
    // Model transform should already be applied to the vertex on the host side.
    mat4 model = mat4(1.0);

    v_Normal = mat3(Transpose(Inverse(model))) * a_Normal;
    // Simpler version, No need for custom inverse & transpose functions on ES2.0.
    // v_Normal = a_Normal;

    v_Color = a_Color;

    // v_FragPos = vec3(model * vec4(a_Position, 1.0));
    // gl_Position = u_Projection * u_View * model * vec4(a_Position, 1.0);
    // Unnecessary vector multplication if model matrix is the identity matrix.
    v_FragPos   = a_Position;
    gl_Position = u_Projection * u_View * vec4(a_Position, 1.0);
}

#shader fragment

#ifdef GL_ES
precision mediump float;
#endif

uniform float u_Ambient;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

varying vec3 v_Normal;
varying vec3 v_FragPos;
varying vec4 v_Color;

void main()
{
    // lighting
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

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
    // vec3 light   = ambient + diffuse + specular;
    vec3 light = ambient + diffuse;

    gl_FragColor = vec4(light, 1.0) * v_Color;
}
