#version 410

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in  vec4 vertex_color;
layout(location=1) out  vec4 color;

void main() {
    gl_Position = a_vertex_position;
    color = vertex_color;
}
