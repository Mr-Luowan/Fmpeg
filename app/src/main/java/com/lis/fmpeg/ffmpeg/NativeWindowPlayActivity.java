package com.lis.fmpeg.ffmpeg;

import static com.lis.fmpeg.ffmpeg.FfmpegUtils.VIDEO_RENDER_AN_WINDOW;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;

import com.lis.fmpeg.R;

public class NativeWindowPlayActivity extends AppCompatActivity implements SurfaceHolder.Callback, EventCallback {
    private static final String TAG = "NativeWindowPlayAc";
    private String mVideoPath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/video/one_piece.mp4";

    private MySurfaceView mSurfaceView = null;
    private FfmpegUtils mFfmpegUtils;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_native_window_play);
        mSurfaceView = findViewById(R.id.surface_view);
        mSurfaceView.getHolder().addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        Log.i(TAG, "surfaceCreated: surfaceHolder --> " + surfaceHolder);
        mFfmpegUtils = new FfmpegUtils();
        mFfmpegUtils.addEventCallback(this);
        mFfmpegUtils.init(mVideoPath, VIDEO_RENDER_AN_WINDOW, surfaceHolder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {
        mFfmpegUtils.play();
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
        mFfmpegUtils.stop();
        mFfmpegUtils.unInit();
    }

    @Override
    public void onPlayerEvent(int msgType, float msgValue) {
        Log.d(TAG, "onPlayerEvent() called with: msgType = [" + msgType + "], msgValue = [" + msgValue + "]");
    }
}