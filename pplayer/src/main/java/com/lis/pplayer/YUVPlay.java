package com.lis.pplayer;


import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

public class YUVPlay extends GLSurfaceView {

    private String yuv420pPath;
    private Object surface;

    public YUVPlay(Context context) {
        this(context, null);
    }

    public YUVPlay(Context context, AttributeSet attrs) {
        super(context, attrs);
    }


    public void glesPlay(final String yuv420pPath,final Object surface) {
        this.yuv420pPath = yuv420pPath;
        this.surface = surface;
        Thread thread = new Thread(playRunnable);
        thread.start();
    }

    private Runnable playRunnable = new Runnable() {
        @Override
        public void run() {
            nativeGlesPlay(yuv420pPath, surface);
        }
    };



    public native void nativeGlesPlay(String yuv420pPath, Object surface);

    public native void onDestory();
}
