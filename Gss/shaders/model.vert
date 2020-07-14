#version 330

layout(location=0) in vec3 position;
layout(location=1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool normal_inverse_flag;

void main()
{
    FragPos = vec3(model * vec4(position, 1.0));
    if(normal_inverse_flag)
        Normal = mat3(transpose(inverse(model))) * -aNormal;
    else
        Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * model * vec4(position, 1.0f);
}
