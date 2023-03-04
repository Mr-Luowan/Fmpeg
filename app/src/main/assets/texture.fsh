#version 300 es
precision mediump float;
out vec4 fragColor;
in vec3 outColor;
in vec2 TexCoord;
uniform sampler2D texture1;
void main()
{
    fragColor = texture(texture1, TexCoord);
}