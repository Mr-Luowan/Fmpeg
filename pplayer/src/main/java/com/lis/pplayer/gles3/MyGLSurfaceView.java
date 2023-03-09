package com.lis.pplayer.gles3;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import java.io.IOException;
import java.io.InputStream;

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
        if (index == 3) {
            loadNV21Image();
        }
        //JavaGLRender javaGLRender = new JavaGLRender();
        setRenderer(mGlRender);
        setRenderMode(RENDERMODE_CONTINUOUSLY);
    }

    private void loadNV21Image() {
        InputStream is = null;
        try {
            is = getContext().getAssets().open("img/YUV_Image_840x1074.NV21");
        } catch (IOException e) {
            e.printStackTrace();
        }

        int lenght = 0;
        try {
            lenght = is.available();
            byte[] buffer = new byte[lenght];
            is.read(buffer);
            mGlRender.native_setImageData(IMAGE_FORMAT_NV21, 840, 1074, buffer);
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try
            {
                is.close();
            }
            catch(IOException e)
            {
                e.printStackTrace();
            }
        }
    }

}
