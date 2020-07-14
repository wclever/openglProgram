#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
    vec3 normal;
    vec4 color;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform bool normal_inverse_flag;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    if(normal_inverse_flag)
        vs_out.normal = vec3(projection * vec4(normalMatrix * -aNormal, 0.0));
    else
        vs_out.normal = vec3(projection * vec4(normalMatrix * aNormal, 0.0));
    vs_out.color = vec4(aPos, 0.0) * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
