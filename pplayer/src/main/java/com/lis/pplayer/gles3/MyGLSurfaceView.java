package com.lis.pplayer.gles3;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;

import java.io.IOException;
import java.io.InputStream;

public class MyGLSurfaceView extends GLSurfaceView implements ScaleGestureDetector.OnScaleGestureListener {
    private static final String TAG = "MyGLSurfaceView";

    private final float TOUCH_SCALE_FACTOR = 180.0f / 320;

    public static final int IMAGE_FORMAT_RGBA = 0x01;
    public static final int IMAGE_FORMAT_NV21 = 0x02;
    public static final int IMAGE_FORMAT_NV12 = 0x03;
    public static final int IMAGE_FORMAT_I420 = 0x04;

    private long mLastMultiTouchTime = 0;
    private float mPreviousX = 0;
    private float mPreviousY = 0;
    private float mXAngle = 0;
    private float mYAngle = 0;
    float mPreScale = 1.0f;
    float mCurScale = 1.0f;
    private NativeRender mGlRender;

    private ScaleGestureDetector mScaleGestureDetector;

    public MyGLSurfaceView(Context context) {
        this(context, 0);
    }

    public MyGLSurfaceView(Context context, int index) {
        this(context, null, index);
    }

    public MyGLSurfaceView(Context context, AttributeSet attrs, int index) {
        super(context, attrs);
        setEGLContextClientVersion(3);
        mGlRender = new NativeRender(index);
        if (index == 3) {
            loadNV21Image();
        }
        //JavaGLRender javaGLRender = new JavaGLRender();
        setRenderer(mGlRender);
        setRenderMode(RENDERMODE_CONTINUOUSLY);
        mScaleGestureDetector = new ScaleGestureDetector(context, this);
    }

    private void loadNV21Image() {
        InputStream is = null;
        try {
            is = getContext().getAssets().open("img/YUV_Image_840x1074.NV21");
        } catch (IOException e) {
            e.printStackTrace();
        }

        int lenght = 0;
        try {
            lenght = is.available();
            byte[] buffer = new byte[lenght];
            is.read(buffer);
            mGlRender.native_setImageData(IMAGE_FORMAT_NV21, 840, 1074, buffer);
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                is.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (event.getPointerCount() == 1) {
            consumeTouchEvent(event);
            long currentTimeMillis = System.currentTimeMillis();
            if (currentTimeMillis - mLastMultiTouchTime > 200) {
                float x = event.getX();
                float y = event.getY();
                switch (event.getAction()) {
                    case MotionEvent.ACTION_MOVE:
                        float dx = x - mPreviousX;
                        float dy = y - mPreviousY;
                        mYAngle += dx * TOUCH_SCALE_FACTOR;
                        mXAngle += dy * TOUCH_SCALE_FACTOR;
                }
                mPreviousX = x;
                mPreviousY = y;
                mGlRender.updateTransformMatrix(mXAngle, mYAngle, mCurScale, mCurScale);
                requestRender();
            }
        } else {
            mScaleGestureDetector.onTouchEvent(event);
        }
        return true;
    }

    public void consumeTouchEvent(MotionEvent e) {
        dealClickEvent(e);
        float touchX = -1, touchY = -1;
        switch (e.getAction()) {
            case MotionEvent.ACTION_MOVE:
                touchX = e.getX();
                touchY = e.getY();
                break;
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_CANCEL:
                touchX = -1;
                touchY = -1;
                break;
            default:
                break;
        }

        //滑动、触摸
        //mGLRender.setTouchLoc(touchX, touchY);
        //requestRender();
    }

    public void dealClickEvent(MotionEvent e) {
        float touchX = -1, touchY = -1;
        switch (e.getAction()) {
            case MotionEvent.ACTION_UP:
                touchX = e.getX();
                touchY = e.getY();
            {
                //点击
                //mGLRender.setTouchLoc(touchX, touchY);
            }
            break;
            default:
                break;
        }
    }


    @Override
    public boolean onScale(ScaleGestureDetector detector) {
        float preSpan = detector.getPreviousSpan();
        float curSpan = detector.getCurrentSpan();
        Log.i(TAG, "onScale: 缩放中...preSpan = " + preSpan + "   curSpan = " + curSpan );
        if (curSpan < preSpan) {
            mCurScale = mPreScale - (preSpan - curSpan) / 200;
        } else {
            mCurScale = mPreScale + (curSpan - preSpan) / 200;
        }
        mCurScale = Math.max(0.05f, Math.min(mCurScale, 80.0f));
        mGlRender.updateTransformMatrix(mXAngle, mYAngle, mCurScale, mCurScale);
        requestRender();
        return false;
    }

    @Override
    public boolean onScaleBegin(ScaleGestureDetector detector) {
        Log.i(TAG, "onScaleBegin: 缩放开始...");
        return true;
    }

    @Override
    public void onScaleEnd(ScaleGestureDetector detector) {
        Log.i(TAG, "onScaleEnd: 缩放结束...");
        mPreScale = mCurScale;
        mLastMultiTouchTime = System.currentTimeMillis();
    }

    public void testNative()
    {
        if (mGlRender != null) {
            mGlRender.testNative();
        }
    }
}
