package com.lis.pplayer.util;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import com.lis.pplayer.GlobalContext;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

public class ResourceUtils {
    private static final String TAG = "ResourceUtils";

    public static byte[] getPicData(String resName) throws IOException {
        Context globalContext = GlobalContext.getGlobalContext();
        int drawable = globalContext.getResources().getIdentifier(resName, "drawable", globalContext.getPackageName());
        Bitmap bitmap;
        try (InputStream is = globalContext.getResources().openRawResource(drawable)) {
            bitmap = BitmapFactory.decodeStream(is);
            if (bitmap != null) {
                int bytes = bitmap.getByteCount();
                ByteBuffer buf = ByteBuffer.allocate(bytes);
                bitmap.copyPixelsToBuffer(buf);
                return buf.array();
            }
        }
        return null;
    }

    public static void sayHello() {
        Log.i(TAG, "sayHello: Native 调用java 方法");
    }

}
