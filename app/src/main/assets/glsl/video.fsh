#version 300 es
precision mediump float;
in vec2 v_texCoord;
layout(location = 0) out vec4 outColor;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
void main()
{
    outColor = texture(texture0, v_texCoord);
}