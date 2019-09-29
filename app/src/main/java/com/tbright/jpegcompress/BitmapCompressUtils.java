package com.tbright.jpegcompress;

import android.graphics.Bitmap;

public class BitmapCompressUtils {
    static {
        System.loadLibrary("compress-jpeg");
    }
    public static native int nativeCompress(Bitmap bitmap,int quality,String path);
    public static native int nativeCompressWithSize(Bitmap bitmap,int quality,String path,int width ,int height);
}
