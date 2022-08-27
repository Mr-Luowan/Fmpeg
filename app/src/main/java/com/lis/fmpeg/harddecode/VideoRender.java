package com.lis.fmpeg.harddecode;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.util.Log;

import com.lis.fmpeg.utils.GLTools;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class VideoRender implements GLSurfaceView.Renderer {
    private static final String TAG = "VideoRender";
    private final Context mContext;
    private VideoProgram mVideoProgram;

    public VideoRender(Context context) {
        mContext = context;
        mVideoProgram = new VideoProgram(mContext);
    }

    public SurfaceTexture getSurfaceTexture() {
        if (mVideoProgram != null) {
            return mVideoProgram.getSurfaceTexture();
        }
        return null;
    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        GLES20.glClearColor(0f, 0f, 0f, 0f);
        //开启混合，即半透明
        GLES20.glEnable(GLES20.GL_BLEND);
        GLES20.glBlendFunc(GLES20.GL_SRC_ALPHA, GLES20.GL_ONE_MINUS_SRC_ALPHA);
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int width, int height) {
        GLES20.glViewport(0, 0, width, height);

        Log.e(TAG, "onSurfaceChanged宽度: width ==> " + width);
        Log.e(TAG, "onSurfaceChanged宽度: height ==> " + height);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT|GLES20.GL_DEPTH_BUFFER_BIT);
        mVideoProgram.onDraw();
    }
}
