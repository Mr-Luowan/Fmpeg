package com.lis.fmpeg;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.TextView;

import com.lis.fmpeg.audio.AudioActivity;
import com.lis.fmpeg.ffmpeg.NativeWindowPlayActivity;
import com.lis.fmpeg.harddecode.GLVideoActivity;
import com.lis.fmpeg.harddecode.MediaCodecDecodeActivity;
import com.lis.fmpeg.opengl.GLESActivity;
import com.lis.fmpeg.opengl.OpenGLVideoActivity;
import com.lis.fmpeg.utils.CommonUtils;
import com.lis.pplayer.FfmpegUtils;

public class MainActivity extends AppCompatActivity {

    private static final String[] REQUEST_PERMISSIONS = {
            Manifest.permission.CAMERA,
            Manifest.permission.RECORD_AUDIO,
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
    };
    private static final int PERMISSION_REQUEST_CODE = 1;

    private FfmpegUtils mFfmpegUtils;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView tv = findViewById(R.id.sample_text);
        tv.setText(FfmpegUtils.getFFmpegVersion());
        setNativeAssetsManager(getAssets());
        mFfmpegUtils = new FfmpegUtils();
    }



    @Override
    protected void onResume() {
        super.onResume();
        if (!hasPermissionsGranted(REQUEST_PERMISSIONS)) {
            ActivityCompat.requestPermissions(this, REQUEST_PERMISSIONS, PERMISSION_REQUEST_CODE);
        }
        CommonUtils.copyAssetsDirToPhone(this, "video", getExternalFilesDir(null).getPath());
    }

    private boolean hasPermissionsGranted(String[] permissions) {
        for (String permission : permissions) {
            if (ActivityCompat.checkSelfPermission(this, permission)
                    != PackageManager.PERMISSION_GRANTED) {
                return false;
            }
        }
        return true;
    }

    public void goANativeWindowPage(View v) {
        mFfmpegUtils.test("1");
//        startActivity(new Intent(MainActivity.this, NativeWindowPlayActivity.class));
    }
    public void mediaCodecDecode(View v) {
        startActivity(new Intent(MainActivity.this, MediaCodecDecodeActivity.class));
    }
    public void openGLMediaCodecDecode(View v) {
        startActivity(new Intent(MainActivity.this, GLVideoActivity.class));
    }
    public void audioDealPage(View v) {
        startActivity(new Intent(MainActivity.this, AudioActivity.class));
    }
    public void goNativeOpenglESPage(View v) {
        startActivity(new Intent(MainActivity.this, OpenGLVideoActivity.class));
    }
    public void goGLESPage(View v) {
        startActivity(new Intent(MainActivity.this, GLESActivity.class));
    }


    public native void setNativeAssetsManager(AssetManager assetsManager);

}