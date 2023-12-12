package com.lis.fmpeg.audio;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.lis.fmpeg.R;
import com.lis.pplayer.NativePCMPlayer;

import java.io.File;

public class AudioActivity extends AppCompatActivity {
    private static final String TAG = "AudioActivity";
    private AudioRecorder mAudioRecorder;
    private NativePCMPlayer mNativePCMPlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_audio);
    }

    public void playPCMVoice(View v) {
        String pcmFilePath = getExternalFilesDir(null).getPath() + "/audio/MyTestPCM.pcm";
        if (!new File(pcmFilePath).exists())
        {
            Toast.makeText(this, "目标文件不存在", Toast.LENGTH_SHORT).show();
            return;
        }
        AudioTracker audioTracker = new AudioTracker(this);
        audioTracker.createAudioTracker(pcmFilePath);
        audioTracker.start();
    }

    public void recordPCMVoice(View v) {
        String pcmFilePath = getExternalFilesDir(null).getPath() + "/audio/MyTestPCM.pcm";
        if (mAudioRecorder == null) {
            mAudioRecorder = new AudioRecorder(this);
            mAudioRecorder.createAudioRecorder(pcmFilePath);
        }
        if (mAudioRecorder.mStatus != AudioRecorder.Status.STATUS_START) {
            mAudioRecorder.record();
            ((TextView) v).setText("停止录制");
        } else {
            mAudioRecorder.stopRecord();
            ((TextView) v).setText("开始录制");
        }
    }

    public void native_playPCMVoice(View v) {
        String pcmFilePath = getExternalFilesDir(null).getPath() + "/audio/MyTestPCM.pcm";
        if (!new File(pcmFilePath).exists())
        {
            Toast.makeText(this, "目标文件不存在", Toast.LENGTH_SHORT).show();
            return;
        }
        mNativePCMPlayer = new NativePCMPlayer();
        mNativePCMPlayer.playPCMVoice(pcmFilePath);
        NativePCMPlayer.PlayStatus status = mNativePCMPlayer.getStatus();
        Log.i(TAG, "播放器状态 status =  " + status);
    }


    public void native_recordVoice(View v) {
        String pcmFilePath = getExternalFilesDir(null).getPath() + "/audio/a.txt";
        if (!new File(pcmFilePath).exists())
        {
            Toast.makeText(this, "目标文件不存在", Toast.LENGTH_SHORT).show();
            return;
        }
        mNativePCMPlayer = new NativePCMPlayer();
        mNativePCMPlayer.recordPCMVoice(pcmFilePath);
    }

    public void native_pausePCMVoice(View v) {
        NativePCMPlayer.PlayStatus status = mNativePCMPlayer.getStatus();
        if (status == NativePCMPlayer.PlayStatus.SL_PLAYSTATE_PLAYING) {
            mNativePCMPlayer.pausePCMVoice();
            ((TextView)v).setText("C++继续播放PCM");
        } else {
            mNativePCMPlayer.reStartNativePlayer();
            ((TextView)v).setText("C++暂停播放PCM");

        }
        Log.i(TAG, "播放器状态 status =  " + status);
    }

    public void native_stopPCMVoice(View v) {
        mNativePCMPlayer.stopPCMVoice();
        NativePCMPlayer.PlayStatus status = mNativePCMPlayer.getStatus();
        Log.i(TAG, "播放器状态 status =  " + status);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mAudioRecorder != null) {
            mAudioRecorder.release();
        }
    }
}