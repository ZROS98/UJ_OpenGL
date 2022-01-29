#version 460

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in  vec4 vertex_color;
layout(location=1) out  vec4 color;

layout(std140, binding=1) uniform Transformations {
    mat4 PVM;
};

void main() {
    gl_Position =  PVM * a_vertex_position;
    color = vertex_color;
}
