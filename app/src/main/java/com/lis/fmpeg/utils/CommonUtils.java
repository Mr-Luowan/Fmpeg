package com.lis.fmpeg.utils;

import android.content.Context;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class CommonUtils {

    private static final String TAG = "CommonUtils";

    public static void copyAssetsDirToPhone(Context context, String assetsDirName, String targetPath) {
        try {
            Log.i(TAG, "copyAssetsDirToPhone: assetsPath -->" + assetsDirName + "    phonePath -->" + targetPath);
            String[] list = context.getAssets().list(assetsDirName);
            if (list == null) {
                return;
            }
            if (list.length == 0) {
                InputStream inputStream = context.getAssets().open(assetsDirName);
                byte[] buf = new byte[1024];
                int bt = 0;
                File file = new File(
                        targetPath
                                + File.separator
                                + assetsDirName.substring(assetsDirName.lastIndexOf("/")));
                if (!file.exists()) {
                    file.createNewFile();
                } else {
                    return;
                }
                FileOutputStream fileOutputStream = new FileOutputStream(file);
                while ((bt = inputStream.read(buf)) != -1) {
                    fileOutputStream.write(buf, 0, bt);
                }
                fileOutputStream.flush();
                inputStream.close();
                fileOutputStream.close();
            } else {
                String subDirName = assetsDirName;
                if (assetsDirName.contains("/")) {
                    subDirName = assetsDirName.substring(assetsDirName.lastIndexOf('/') + 1);
                }
                targetPath = targetPath + File.separator + subDirName;
                File file = new File(targetPath);
                if (!file.exists()) {
                    file.mkdir();
                }
                for (String s : list) {
                    copyAssetsDirToPhone(context, assetsDirName + File.separator + s, targetPath);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
