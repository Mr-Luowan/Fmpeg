package com.lis.fmpeg.ffmpeg;

public interface EventCallback {

    void onPlayerEvent(int msgType, float msgValue);
}
