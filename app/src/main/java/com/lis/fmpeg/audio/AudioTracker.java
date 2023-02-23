package com.lis.fmpeg.audio;

import android.content.Context;
import android.media.AudioAttributes;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.widget.Toast;

import java.io.BufferedInputStream;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class AudioTracker {
    private static final String TAG = "AudioTracker";
    //采样率 44.1K HZ
    private final static int SAMPLE_RATE = 44100;
    //单声道
    private final static int CHANNEL = AudioFormat.CHANNEL_OUT_MONO;
    //位深度 16位
    private final static int AUDIO_FORMAT = AudioFormat.ENCODING_PCM_16BIT;
    //缓冲区大小
    private int mBufferSizeInBytes = 0;
    //播放器对象
    private AudioTrack mAudioTrack;
    //文件路径
    private String mFilePath;
    private volatile Status mStatus = Status.STATUS_NO_READY;
    //单线程线程池
    private ExecutorService mExecutorService = Executors.newSingleThreadExecutor();
    private Handler mHandler = new Handler(Looper.getMainLooper());
    private Context mContext;

    public AudioTracker(Context context) {
        mContext = context;
    }


    public void createAudioTracker(String filePath) throws IllegalStateException {
        mFilePath = filePath;
        mBufferSizeInBytes = AudioTrack.getMinBufferSize(SAMPLE_RATE, CHANNEL, AUDIO_FORMAT);
        if (mBufferSizeInBytes < 0) {
            throw new IllegalStateException("AudioTrack is not available!");
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            AudioAttributes audioAttributes = new AudioAttributes.Builder()
                    .setUsage(AudioAttributes.USAGE_MEDIA)
                    .setContentType(AudioAttributes.CONTENT_TYPE_MUSIC)
                    .setLegacyStreamType(AudioManager.STREAM_MUSIC)
                    .build();
            AudioFormat audioFormat = new AudioFormat.Builder()
                    .setSampleRate(SAMPLE_RATE)
                    .setChannelIndexMask(CHANNEL)
                    .setEncoding(AUDIO_FORMAT)
                    .build();
            mAudioTrack = new AudioTrack.Builder()
                    .setAudioAttributes(audioAttributes)
                    .setAudioFormat(audioFormat)
                    .setBufferSizeInBytes(mBufferSizeInBytes)
                    .setTransferMode(AudioTrack.MODE_STREAM)
                    .build();
        } else {
            mAudioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, SAMPLE_RATE, CHANNEL,
                    AUDIO_FORMAT, mBufferSizeInBytes, AudioTrack.MODE_STREAM);
        }
        mStatus = Status.STATUS_READY;
    }

    public void start() {
        if (mStatus == Status.STATUS_NO_READY || mAudioTrack == null) {
            throw new IllegalStateException("AudioTrack is not init!");
        }
        if (mStatus == Status.STATUS_START) {
            throw new IllegalStateException("AudioTrack is playing! can not play again");
        }
        mExecutorService.execute(new Runnable() {
            @Override
            public void run() {
                try {
                    playAudioData();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        mStatus = Status.STATUS_START;
    }

    private void playAudioData() throws IOException {
        try (InputStream dis = new DataInputStream(new BufferedInputStream(new FileInputStream(mFilePath)))) {
            byte[] bytes = new byte[mBufferSizeInBytes];
            int length;
            //没有在播放 并且初始化成功
            if (mAudioTrack != null &&
                    mAudioTrack.getState() != AudioTrack.STATE_UNINITIALIZED &&
                    mAudioTrack.getState() != AudioTrack.PLAYSTATE_PLAYING) {
                mAudioTrack.play();
            }
            //write是阻塞方法
            while ((length = dis.read(bytes)) != -1 && mStatus == Status.STATUS_START) {
                mAudioTrack.write(bytes, 0, length);
            }
            mHandler.post(() -> Toast.makeText(mContext, "播放完成", Toast.LENGTH_SHORT).show());
        }
    }

    public void stop() throws IllegalStateException {
        if (mStatus == Status.STATUS_NO_READY ||
                mStatus == Status.STATUS_READY) {
            Log.d(TAG, "stop: 播放尚未开始");
        } else {
            mStatus = Status.STATUS_STOP;
            mAudioTrack.stop();
            release();
        }
    }

    private void release() {
        mStatus = Status.STATUS_NO_READY;
        if (mAudioTrack != null) {
            mAudioTrack.release();
            mAudioTrack = null;
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
