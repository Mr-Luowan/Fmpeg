package com.lis.pplayer;

import android.content.Context;

public class GlobalContext {

    private static Context sGlobalContext;

    public static Context getGlobalContext() {
        return sGlobalContext;
    }


    public static void setGlobalContext(Context globalContext) {
        if (sGlobalContext != null) {
            return;
        }
        sGlobalContext = globalContext;
    }
}
