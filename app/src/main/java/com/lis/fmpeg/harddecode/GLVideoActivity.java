package com.lis.fmpeg.harddecode;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.SurfaceTexture;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Environment;

import com.lis.fmpeg.R;

public class GLVideoActivity extends AppCompatActivity {

    private HardDecode mHardDecode;
    private final String mVideoPath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/video/one_piece.mp4";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_glvideo);
        GLSurfaceView surfaceView = findViewById(R.id.gl_surface_view);
        surfaceView.setEGLContextClientVersion(2);
        VideoRender videoRender = new VideoRender(this);
        surfaceView.setRenderer(videoRender);
        SurfaceTexture surfaceTexture = videoRender.getSurfaceTexture();
        mHardDecode = new HardDecode();
        mHardDecode.setVideoPath(mVideoPath);
        mHardDecode.setSurfaceTexture(surfaceTexture);

        findViewById(R.id.button_start).setOnClickListener(v -> {
            if (mHardDecode != null) {
                mHardDecode.start();
            }
        });
    }
}