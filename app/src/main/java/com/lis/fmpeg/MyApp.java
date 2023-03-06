package com.lis.fmpeg;

import android.app.Application;

import com.lis.pplayer.GlobalContext;

public class MyApp extends Application {
    @Override
    public void onCreate() {
        super.onCreate();
        GlobalContext.setGlobalContext(this);
    }
}
