package com.lis.fmpeg.opengl;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.ViewGroup;
import android.widget.LinearLayout;

import com.lis.fmpeg.R;
import com.lis.fmpeg.ffmpeg.MySurfaceView;
import com.lis.pplayer.gles3.MyGLSurfaceView;

import java.io.IOException;
import java.io.InputStream;

public class GLESActivity extends AppCompatActivity {
    private static final String TAG = "GLESActivity";
    private MyGLSurfaceView surface;
    private ViewGroup mRootView;

    @Override
    @SuppressLint("MissingInflatedId")
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_glesactivity);
        mRootView = findViewById(R.id.root);
        surface = new MyGLSurfaceView(this, 1);
        mRootView.addView(surface);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.gles_menu, menu);
        return true;
    }

    @SuppressLint("NonConstantResourceId")
    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        switch (item.getItemId()) {
            case R.id.triangle: {
                Log.d(TAG, "onOptionsItemSelected:  = triangle");
                reAddContentView(0);
                break;
            }
            case R.id.pic: {
                Log.d(TAG, "onOptionsItemSelected:  = pic");
                reAddContentView(1);
                break;
            }
            case R.id.cube: {
                Log.d(TAG, "onOptionsItemSelected:  = cube");
                reAddContentView(2);
                break;
            }
            case R.id.nv21pic: {
                Log.d(TAG, "onOptionsItemSelected:  = nv21pic");
                reAddContentView(3);
                break;
            }
        }
        return super.onOptionsItemSelected(item);
    }

    private void reAddContentView(int type) {
        mRootView.removeView(surface);
        surface = new MyGLSurfaceView(this, type);
        LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        mRootView.addView(surface, lp);
    }


}