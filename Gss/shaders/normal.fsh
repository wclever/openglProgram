#version 330 core
out vec4 FragColor;

in GS_OUT {
    vec3 normal;
    vec4 color;
} fs_in;

void main()
{
    //FragColor = vec4(1.0, 1.0, 0.0, 1.0);
    FragColor = fs_in.color * abs(normalize(fs_in.normal).z);
}
