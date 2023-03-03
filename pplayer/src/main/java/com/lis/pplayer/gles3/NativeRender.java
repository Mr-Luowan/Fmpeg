package com.lis.pplayer.gles3;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class NativeRender implements GLSurfaceView.Renderer {

    public NativeRender() {
        native_OnInit();
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        native_OnSurfaceCreated();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        native_OnSurfaceChanged(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        native_OnDrawFrame();
    }


    public native void native_OnInit();

    public native void native_OnUnInit();

    public native void native_OnSurfaceCreated();

    public native void native_OnSurfaceChanged(int width, int height);

    public native void native_OnDrawFrame();
}