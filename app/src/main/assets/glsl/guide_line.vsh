#version 300 es
layout (location = 0) in vec2 vPosition;
out vec3 oColor;
void main() {
     gl_Position = vec4(vPosition, 0.0f, 1.0f);
     oColor = vec3(1.0f,0.0f,0.0f);
}