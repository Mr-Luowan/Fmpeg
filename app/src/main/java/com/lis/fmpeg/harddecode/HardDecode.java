package com.lis.fmpeg.harddecode;

import android.graphics.SurfaceTexture;
import android.media.MediaCodec;
import android.media.MediaExtractor;
import android.media.MediaFormat;
import android.util.Log;
import android.view.Surface;

import java.io.IOException;
import java.nio.ByteBuffer;

public class HardDecode extends Thread {
    private static final String TAG = "HardDecode";
    private String mVideoPath;
    //音视频分离器
    private MediaExtractor mMediaExtractor;
    private long mDuration = -1L;
    //解码器
    private MediaCodec mCodec;
    private Surface mSurface;
    private MediaCodec.BufferInfo mBufInfo = new MediaCodec.BufferInfo();
    private ByteBuffer[] mInputBuffers;
    private ByteBuffer[] mOutputBuffers;
    private SurfaceTexture mSurfaceTexture;

    public void setSurface(Surface surface) {
        mSurface = surface;
    }

    public void setVideoPath(String videoPath) {
        mVideoPath = videoPath;
    }

    public void setSurfaceTexture(SurfaceTexture surfaceTexture) {
        mSurfaceTexture = surfaceTexture;
    }

    @Override
    public void run() {
        super.run();
        mMediaExtractor = new MediaExtractor();
        int frameIndex = -1;
        try {
            mMediaExtractor.setDataSource(mVideoPath);
            int trackCount = mMediaExtractor.getTrackCount();
            int videoIndex = -1;
            for (int i = 0; i < trackCount; i++) {
                MediaFormat trackFormat = mMediaExtractor.getTrackFormat(i);
                String formatMime = trackFormat.getString(MediaFormat.KEY_MIME);
                if (formatMime.startsWith("video/")) {
                    videoIndex = i;
                    mMediaExtractor.selectTrack(i);
                    break;
                }
            }
            //解码步骤 1：初始化解码器  configure-start
            MediaFormat videoFormat = mMediaExtractor.getTrackFormat(videoIndex);
            String videoMime = videoFormat.getString(MediaFormat.KEY_MIME);
            mDuration = videoFormat.getLong(MediaFormat.KEY_DURATION) / 1000;
            mCodec = MediaCodec.createDecoderByType(videoMime);
            Log.e(TAG, "start: videoMime ==> " + videoMime);
            Log.e(TAG, "start: mDuration ==> " + mDuration);
            if (mSurfaceTexture != null) {
                mSurface = new Surface(mSurfaceTexture);
            }
            Log.e(TAG, "start: mSurfaceTexture ==> " + mSurfaceTexture + "   mSurface=>" + mSurface);
            mCodec.configure(videoFormat, mSurface, null, 0);
            mCodec.start();
            mInputBuffers = mCodec.getInputBuffers();
            mOutputBuffers = mCodec.getOutputBuffers();
        } catch (IOException e) {
            e.printStackTrace();
        }
        boolean isEOF = false;
        while (!Thread.interrupted()) {
            //解码步骤 2 数据压入解码器输入缓冲
            int inputBufIndex = mCodec.dequeueInputBuffer(1000);
            if (!isEOF) {
                if (inputBufIndex >= 0) {
                    ByteBuffer inputBuffer = mInputBuffers[inputBufIndex];
                    int sampleSize = mMediaExtractor.readSampleData(inputBuffer, 0);
                    if (sampleSize < 0) {
                        mCodec.queueInputBuffer(inputBufIndex, 0, 0, 0,
                                MediaCodec.BUFFER_FLAG_END_OF_STREAM);
                        isEOF = true;
                    } else {
                        mCodec.queueInputBuffer(inputBufIndex, 0, sampleSize, mMediaExtractor.getSampleTime(), 0);
                        //继续处理下一帧
                        mMediaExtractor.advance();
                    }
                }
            }
            //解码步骤：3. 将解码好的数据从缓冲区拉取出来
            int outputIndex = mCodec.dequeueOutputBuffer(mBufInfo, 1000);
            switch (outputIndex) {
                case MediaCodec.INFO_OUTPUT_FORMAT_CHANGED: {
                    Log.i(TAG, "dequeueOutputBuffer: MediaCodec.INFO_OUTPUT_FORMAT_CHANGED");
                    break;
                }
                case MediaCodec.INFO_TRY_AGAIN_LATER: {
                    Log.i(TAG, "dequeueOutputBuffer: MediaCodec.INFO_TRY_AGAIN_LATER");
                    break;
                }
                case MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED: {
                    mOutputBuffers = mCodec.getOutputBuffers();
                    break;
                }
                default: {
                    if (outputIndex > 0) {
                        ByteBuffer outputBuffer = mOutputBuffers[outputIndex];
                        frameIndex++;
                        Log.d(TAG, "run: 处理第 " + frameIndex + " 帧");
                        mCodec.releaseOutputBuffer(outputIndex, true);
                    }

                    break;
                }
            }
            if (mBufInfo.flags == MediaCodec.BUFFER_FLAG_END_OF_STREAM) {
                Log.d(TAG, "run: 解码完成");
                break;
            }
        }
        release();
    }


    private void release() {
        if (mMediaExtractor != null) {
            mMediaExtractor.release();
        }
        if (mCodec != null) {
            mCodec.stop();
            mCodec.release();
        }
        Thread.interrupted();
    }
}
