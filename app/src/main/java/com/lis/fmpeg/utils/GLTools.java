package com.lis.fmpeg.utils;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.text.TextUtils;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.charset.StandardCharsets;

public class GLTools {
    private static final String TAG = "GLTools";

    public static FloatBuffer getFloatBuf(float[] floats) {
        FloatBuffer floatBuffer;
        ///每个浮点数:坐标个数* 4字节
        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(floats.length * 4);
        //使用本机硬件设备的字节顺序
        byteBuffer.order(ByteOrder.nativeOrder());
        // 从字节缓冲区创建浮点缓冲区
        floatBuffer = byteBuffer.asFloatBuffer();
        // 将坐标添加到FloatBuffer
        floatBuffer.put(floats);
        //设置缓冲区以读取第一个坐标
        floatBuffer.position(0);
        return floatBuffer;
    }

    /**
     * 加载纹理
     */
    public static int createTextureID(Context ctx, int resId) {
        int[] textureIDs = new int[1];
        GLES20.glGenTextures(1, textureIDs, 0);
        //产生的纹理id的数量,纹理id的数组,偏移量
        Bitmap bitmap = BitmapFactory.decodeResource(ctx.getResources(), resId);
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureIDs[0]);
        GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap, 0);
        bitmap.recycle();
        return textureIDs[0];
    }

    public static int createOESTexture(Context context) {
        int[] textureIDs = new int[1];
        GLES20.glGenTextures(1, textureIDs, 0);
//        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, textureIDs[0]);
        //激活指定纹理单元
//        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        return textureIDs[0];
    }


    public static int initGLProgram(Context context, String ver, String frag) {
        int programID = GLES20.glCreateProgram();
        int verShader = loadShader(context, GLES20.GL_VERTEX_SHADER, ver);
        int fragShader = loadShader(context, GLES20.GL_FRAGMENT_SHADER, frag);
        GLES20.glAttachShader(programID, verShader);
        GLES20.glAttachShader(programID, fragShader);
        return programID;
    }

    public static int loadShader(Context context, int type, String fileName) {
        String shaderCode = "";
        try {
            InputStream is = context.getAssets().open(fileName);
            int lenght = is.available();
            byte[] buf = new byte[lenght];
            is.read(buf);
            shaderCode = new String(buf, StandardCharsets.UTF_8);
        } catch (IOException e) {
            e.printStackTrace();
        }
        if (TextUtils.isEmpty(shaderCode)) {
            Log.e(TAG, "loadShader: error");
            return -1;
        }
        int shader = GLES20.glCreateShader(type);
        GLES20.glShaderSource(shader, shaderCode);
        GLES20.glCompileShader(shader);
        return shader;
    }

}
