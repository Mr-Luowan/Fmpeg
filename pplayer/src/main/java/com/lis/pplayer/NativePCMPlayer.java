package com.lis.pplayer;


public class NativePCMPlayer {
    private long mNativePlayerHandle = -1;

    public void playPCMVoice(String pcmPath) {
        mNativePlayerHandle = native_playPCMVoice(pcmPath);
    }

    public void stopPCMVoice() {
        if (mNativePlayerHandle != -1) {
            native_stopPCMVoice(mNativePlayerHandle);
        }
    }

    public void pausePCMVoice() {
        if (mNativePlayerHandle != -1) {
            native_pausePCMVoice(mNativePlayerHandle);
        }
    }

    public void reStartNativePlayer() {
        native_reStartPCMVoice(mNativePlayerHandle);
    }

    public PlayStatus getStatus() {
        int code = native_getPlayStatus(mNativePlayerHandle);
        switch (code){
            case 0x00000001:
                return PlayStatus.SL_PLAYSTATE_STOPPED;
            case 0x00000002:
                return PlayStatus.SL_PLAYSTATE_PAUSED;
            case 0x00000003:
                return PlayStatus.SL_PLAYSTATE_PLAYING;
        }
        return PlayStatus.SL_PLAYSTATE_STOPPED;
    }



    public native long native_playPCMVoice(String pcmPath);
    private native void native_pausePCMVoice(long playerHandle);
    private native void native_stopPCMVoice(long playerHandle);
    private native void native_reStartPCMVoice(long nativePlayerHandle);
    private native int native_getPlayStatus(long nativePlayerHandle);


    public enum PlayStatus {
        SL_PLAYSTATE_STOPPED(0x00000001),
        SL_PLAYSTATE_PAUSED(0x00000002),
        SL_PLAYSTATE_PLAYING(0x00000003);

        private final int code;

        PlayStatus(int code) {
            this.code = code;
        }

        public int getCode() {
            return code;
        }

        @Override
        public String toString() {
            return "PlayStatus{" +
                    "code=" + code +
                    '}';
        }
    }
}
