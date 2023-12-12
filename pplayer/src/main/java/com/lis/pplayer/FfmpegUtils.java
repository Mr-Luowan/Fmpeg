package com.lis.pplayer;

import android.view.Surface;

public class FfmpegUtils {

    // Used to load the 'fmpeg' library on application startup.
    static {
        System.loadLibrary("fmpeg");
    }

    public static final int VIDEO_RENDER_OPENGL = 0;
    public static final int VIDEO_RENDER_AN_WINDOW = 1;

    //player type
    public static final int FFMEDIA_PLAYER = 0;
    public static final int HWCODEC_PLAYER = 1;

    private long mNativePlayerHandle = 0;
    private EventCallback mEventCallback;

    public void addEventCallback(EventCallback eventCallback) {
        mEventCallback = eventCallback;
    }

    private void playerEventCallback(int msgType, float msgValue) {
        if (mEventCallback != null) {
            mEventCallback.onPlayerEvent(msgType, msgValue);
        }
    }

    public static String getFFmpegVersion() {
        return native_GetFFmpegVersion();
    }

    public void init(String url, int videoRenderType, Surface surface) {
        mNativePlayerHandle = native_Init(url, FFMEDIA_PLAYER, videoRenderType, surface);
    }

    public void play() {
        native_play(mNativePlayerHandle);
    }


    public void pause() {
        native_pause(mNativePlayerHandle);
    }


    public void stop() {
        native_stop(mNativePlayerHandle);
    }

    public void unInit() {
        native_unInit(mNativePlayerHandle);
    }

    public void onSurfaceCreated() {
        native_onSurfaceCreated(mNativePlayerHandle);
    }
    public void onSurfaceChanged( int width, int height) {
        native_onSurfaceChanged(mNativePlayerHandle, width, height);
    }
    public void onDrawFrame() {
        native_onDrawFrame(mNativePlayerHandle);
    }


    public native String stringFromJNI();

    private static native String native_GetFFmpegVersion();

    private native long native_Init(String url, int playPlayer, int renderType, Object surface);

    private native void native_play(long playerHandle);

    private native void native_pause(long playerHandle);

    private native void native_stop(long playerHandle);

    private native void native_unInit(long playerHandle);

    public native void native_onSurfaceCreated(long playerHandle);
    public native void native_onSurfaceChanged(long playerHandle, int width, int height);

    public native void native_onDrawFrame(long playerHandle);

    public native void test(String fileName);

}
