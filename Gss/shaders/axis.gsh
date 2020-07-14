#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 x;
    vec3 y;
    vec3 z;
} gs_in[];

uniform float MAGNITUDE;
out vec3 fColor;

void axis(vec4 position)
{    
    fColor = vec3(1.0, 0.0, 0.0);
    vec4 pos = position;
    gl_Position = pos; // 1:x
    EmitVertex(); 
    gl_Position = pos + vec4( gs_in[0].x, 0.0);
    EmitVertex();
    EndPrimitive();
	
    fColor = vec3(0.0, 1.0, 0.0);
    gl_Position = pos; // 2:y
    EmitVertex();
    gl_Position = pos + vec4( gs_in[0].y, 0.0);
    EmitVertex();
    EndPrimitive();
	
    fColor = vec3(0.0, 0.0, 1.0);
    gl_Position = pos; // 3:z
    EmitVertex();
    gl_Position = pos + vec4( gs_in[0].z, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main() {    
    axis(gl_in[0].gl_Position);
}
