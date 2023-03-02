package com.lis.fmpeg.opengl;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.view.View;

import com.lis.fmpeg.R;
import com.lis.pplayer.YUVPlay;

public class OpenGLVideoActivity extends AppCompatActivity {

    private YUVPlay mYUVPlay;
    private String mPath;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_open_glvideo);
        mYUVPlay = findViewById(R.id.YUVPlay);
        mPath = getExternalFilesDir(null) + "/video/output.yuv";
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mYUVPlay.onDestory();
        mYUVPlay = null;
    }

    /**
     * OpenGL ES 播放 YUV
     *
     * @param view
     */
    public void gles_play(View view) {
        mYUVPlay.glesPlay(mPath, mYUVPlay.getHolder().getSurface());
    }


}