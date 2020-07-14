#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
    vec4 color;
} gs_in[];

out GS_OUT{
    vec3 normal;
    vec4 color;
} gs_out;

uniform float MAGNITUDE;

void GenerateLine(int index)
{
    gl_Position = gl_in[index].gl_Position;
    gs_out.normal = gs_in[index].normal;
    gs_out.color = gs_in[index].color;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE;
    gs_out.normal = gs_in[index].normal;
    gs_out.color = gs_in[index].color;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}
