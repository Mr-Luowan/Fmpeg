package com.lis.fmpeg.harddecode;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.util.Log;

import com.lis.fmpeg.utils.GLTools;

import java.nio.FloatBuffer;

public class VideoProgram {
    private static final String TAG = "VideoProgram";
    // 顶点坐标
    private float[] mVertexCoors = new float[]{
            -1f, -1f,
            1f, -1f,
            -1f, 1f,
            1f, 1f
    };
    //纹理坐标
    private float[] mTextureCoors = new float[]{
            1f, 1f,
            0f, 1f,
            0f, 0f,
            1f, 0f
    };
    private FloatBuffer mVertexBuffer;
    private FloatBuffer mTextureBuffer;
    private SurfaceTexture mSurfaceTexture;
    private int mProgram = -1;
    private int mVertexPosHandler;
    private int mCoordinateHandle;
    private int mTextureHandler;
    private int mTextureID;
    private Context mContext;


    public VideoProgram(Context context) {
        mVertexBuffer = GLTools.getFloatBuf(mVertexCoors);
        mTextureBuffer = GLTools.getFloatBuf(mTextureCoors);
        mContext = context;
    }

    public void initProgram() {
        if (mProgram == -1) {
            mProgram = GLTools.initGLProgram(mContext, "video.vsh", "video.fsh");
            GLES20.glLinkProgram(mProgram);
            mVertexPosHandler = GLES20.glGetAttribLocation(mProgram, "aPosition");
            mCoordinateHandle = GLES20.glGetAttribLocation(mProgram, "aCoordinate");
            mTextureHandler = GLES20.glGetUniformLocation(mProgram, "uTexture");
            mTextureID = GLTools.createOESTexture(mContext);
        }
    }

    public void onDraw() {
        Log.i(TAG, "onDraw: ");
        initProgram();
        GLES20.glUseProgram(mProgram);
        //激活指定纹理单元
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        //绑定纹理ID到纹理单元
        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, mTextureID);
        //将激活的纹理单元传递到着色器里面
        GLES20.glUniform1f(mTextureHandler, 0);
        if (mSurfaceTexture != null) {
            mSurfaceTexture.updateTexImage();
        }
        //启用顶点的句柄
        GLES20.glEnableVertexAttribArray(mVertexPosHandler);
        GLES20.glEnableVertexAttribArray(mCoordinateHandle);
        GLES20.glVertexAttribPointer(mVertexPosHandler, 2, GLES20.GL_FLOAT, false, 0, mVertexBuffer);
        GLES20.glVertexAttribPointer(mCoordinateHandle, 2, GLES20.GL_FLOAT, false, 0, mCoordinateHandle);
        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);
    }

    public SurfaceTexture getSurfaceTexture() {
        mSurfaceTexture = new SurfaceTexture(mTextureID);
        return mSurfaceTexture;
    }
}
