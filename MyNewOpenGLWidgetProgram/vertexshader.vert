#version 330

layout(location=0) in vec3 position;

uniform mat4 mvp_matrix;

void main()
{
    gl_Position = vec4(position, 1.0) * mvp_matrix;
}
