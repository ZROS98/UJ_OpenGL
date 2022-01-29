#version 460

layout(location=0) out vec4 vFragColor;
layout(location=1) in  vec4 vertex_color;

layout(std140, binding = 0) uniform Modifier {
    float strength;
    vec3 color;
};

void main() {
    vFragColor.a = vertex_color.a;
    vFragColor.rgb = strength*color*vertex_color.rgb;
}
