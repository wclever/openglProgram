#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 axismodel;

out vec3 fColor;

void main()
{
    fColor = color;
    gl_Position = projection * view * axismodel * vec4(position, 1.0);
}
