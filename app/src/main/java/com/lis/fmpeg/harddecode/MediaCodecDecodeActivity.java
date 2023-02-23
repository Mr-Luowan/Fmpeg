package com.lis.fmpeg.harddecode;

import android.os.Bundle;
import android.os.Environment;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import androidx.appcompat.app.AppCompatActivity;

import com.lis.fmpeg.R;

public class MediaCodecDecodeActivity extends AppCompatActivity {
    private static final String TAG = "MediaCodecDecode";
    private String mVideoPath;
    private HardDecode mHardDecode;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_media_codec_decode);
        mVideoPath = getExternalFilesDir(null).getAbsolutePath() + "/video/one_piece.mp4";
        SurfaceView surfaceView = findViewById(R.id.surface_view);
        surfaceView.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(SurfaceHolder surfaceHolder) {
                mHardDecode = new HardDecode();
                mHardDecode.setSurface(surfaceHolder.getSurface());
                mHardDecode.setVideoPath(mVideoPath);
            }

            @Override
            public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {

            }

            @Override
            public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
                if (mHardDecode != null) {
                    mHardDecode.interrupt();
                }
            }
        });
        findViewById(R.id.button).setOnClickListener(v -> {
            if (mHardDecode != null) {
                mHardDecode.start();
            }
        });
    }
}