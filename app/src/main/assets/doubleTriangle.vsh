#version 300 es
layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec3 aColor;
out vec3 oColor;
void main() {
     gl_Position = vec4(vPosition, 0.0f, 1.0f);
     oColor = aColor;
}