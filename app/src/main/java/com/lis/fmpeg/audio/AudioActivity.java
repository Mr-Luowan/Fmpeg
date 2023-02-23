package com.lis.fmpeg.audio;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.lis.fmpeg.R;

import java.io.File;

public class AudioActivity extends AppCompatActivity {
    private static final String TAG = "AudioActivity";
    private AudioRecorder mAudioRecorder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_audioo);
    }

    public void playPCMVoice(View v) {
        String pcmFilePath = getExternalFilesDir(null).getPath() + "/audio/MyTestPCM.pcm";
        AudioTracker audioTracker = new AudioTracker(this);
        audioTracker.createAudioTracker(pcmFilePath);
        audioTracker.start();
    }

    public void recordPCMVoice(View v) {
        String pcmFilePath = getExternalFilesDir(null).getPath() + "/audio/MyTestPCM.pcm";
        File tempFile = new File(pcmFilePath);
        if (tempFile.exists()) {
            boolean delete = tempFile.delete();
            Log.d(TAG, "recordPCMVoice: is delete = " + delete);
        }
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

}