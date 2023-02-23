package com.lis.fmpeg.audio;

import android.annotation.SuppressLint;
import android.content.Context;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.os.Handler;
import android.os.Looper;
import android.widget.Toast;

import java.io.DataOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class AudioRecorder {
    private static final String TAG = "AudioTracker";
    //采样率 44.1K HZ
    private final static int SAMPLE_RATE = 44100;
    //单声道
    private final static int CHANNEL = AudioFormat.CHANNEL_IN_MONO;
    //位深度 16位
    private final static int AUDIO_FORMAT = AudioFormat.ENCODING_PCM_16BIT;
    //缓冲区大小
    private int mBufferSizeInBytes = 0;
    //播放器对象
    private AudioRecord mAudioRecord;
    //文件路径
    private String mFilePath;
    public volatile Status mStatus = Status.STATUS_NO_READY;
    //单线程线程池
    private final ExecutorService mExecutorService = Executors.newSingleThreadExecutor();
    private final Handler mHandler = new Handler(Looper.getMainLooper());
    private final Context mContext;

    public AudioRecorder(Context context) {
        mContext = context;
    }

    @SuppressLint("MissingPermission")
    public void createAudioRecorder(String filePath) {
        mFilePath = filePath;
        mBufferSizeInBytes = AudioRecord.getMinBufferSize(SAMPLE_RATE, CHANNEL, AUDIO_FORMAT);
        mAudioRecord = new AudioRecord(MediaRecorder.AudioSource.MIC, SAMPLE_RATE,
                CHANNEL, AUDIO_FORMAT, mBufferSizeInBytes);
        mStatus = Status.STATUS_READY;
    }

    public void record() {
        if (mStatus != Status.STATUS_READY || mAudioRecord == null) {
            throw new IllegalStateException("mAudioRecord init failed!");
        }
        if (mStatus == Status.STATUS_START) {
            throw new IllegalStateException("status is recording");
        }
        mExecutorService.execute(() -> {
            try {
                recordAudioData();
            } catch (IOException e) {
                e.printStackTrace();
            }
        });
    }

    private void recordAudioData() throws IOException {
        try (OutputStream dos = new DataOutputStream(new FileOutputStream(mFilePath))) {
            byte[] bytes = new byte[mBufferSizeInBytes];
            mAudioRecord.startRecording();
            mStatus = Status.STATUS_START;
            while (mStatus == Status.STATUS_START) {
                int bufferReadResult = mAudioRecord.read(bytes, 0, mBufferSizeInBytes);
                if (bufferReadResult != AudioRecord.ERROR_INVALID_OPERATION) {
                    dos.write(bytes);
                }
            }
        }
    }

    public void stopRecord() {
        mStatus = Status.STATUS_STOP;
        if (mAudioRecord != null && mAudioRecord.getRecordingState() == AudioRecord.RECORDSTATE_RECORDING) {
            mAudioRecord.stop();
        }
        release();
        mHandler.post(() -> Toast.makeText(mContext, "录制完成", Toast.LENGTH_SHORT).show());
    }

    private void release() {
        mStatus = Status.STATUS_NO_READY;
        if (mAudioRecord != null) {
            mAudioRecord.release();
            mAudioRecord = null;
        }
    }

    public enum Status {
        //未准备
        STATUS_NO_READY,
        //准备
        STATUS_READY,
        //播放
        STATUS_START,
        //停止
        STATUS_STOP
    }
}
