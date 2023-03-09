package com.lis.pplayer.gles3;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

public class MyGLSurfaceView extends GLSurfaceView {
    private static final String TAG = "MyGLSurfaceView";
    public static final int IMAGE_FORMAT_RGBA = 0x01;
    public static final int IMAGE_FORMAT_NV21 = 0x02;
    public static final int IMAGE_FORMAT_NV12 = 0x03;
    public static final int IMAGE_FORMAT_I420 = 0x04;
    private NativeRender mGlRender;

    public MyGLSurfaceView(Context context) {
        this(context, 0);
    }

    public MyGLSurfaceView(Context context, int index) {
        this(context, null, index);
    }

    public MyGLSurfaceView(Context context, AttributeSet attrs, int index) {
        super(context, attrs);
        setEGLContextClientVersion(3);
        mGlRender = new NativeRender(index);
        //JavaGLRender javaGLRender = new JavaGLRender();
        setRenderer(mGlRender);
        setRenderMode(RENDERMODE_CONTINUOUSLY);
    }

}
