attribute vec4 aPosition;   //顶点坐标
attribute vec2 aTextCoord;  //纹理坐标
varying vec2 vTextCoord;    //输出纹理坐标给片段着色器

int main()
{
    //图片上下翻转
    vTextCoord = vec2(aTextCoord.x, 1.0 - aTextCoord.y);
    gl_Position = aPosition;
}