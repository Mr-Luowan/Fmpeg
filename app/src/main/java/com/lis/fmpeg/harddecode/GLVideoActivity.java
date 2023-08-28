package com.lis.fmpeg.harddecode;

import androidx.appcompat.app.AppCompatActivity;

import android.opengl.GLSurfaceView;
import android.os.Bundle;

import com.lis.fmpeg.R;
import com.lis.pplayer.FfmpegUtils;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GLVideoActivity extends AppCompatActivity {
    FfmpegUtils mFfmpegUtils;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_glvideo);
        String videoPath = getExternalFilesDir(null).getAbsolutePath() + "/video/test.mp4";
        GLSurfaceView surfaceView = findViewById(R.id.gl_surface_view);
        surfaceView.setEGLContextClientVersion(3);
        surfaceView.setRenderer(new GLSurfaceView.Renderer() {
            @Override
            public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
                mFfmpegUtils = new FfmpegUtils();
                mFfmpegUtils.init(videoPath, FfmpegUtils.VIDEO_RENDER_OPENGL, null);
                mFfmpegUtils.onSurfaceCreated();
            }

            @Override
            public void onSurfaceChanged(GL10 gl10, int i, int i1) {
                mFfmpegUtils.onSurfaceChanged(i, i1);
            }

            @Override
            public void onDrawFrame(GL10 gl10) {
                mFfmpegUtils.onDrawFrame();
            }
        });
        findViewById(R.id.button_start).setOnClickListener(v -> {
            if (mFfmpegUtils != null) {
                mFfmpegUtils.play();
            }
        });
    }
}