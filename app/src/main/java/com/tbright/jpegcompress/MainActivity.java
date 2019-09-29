package com.tbright.jpegcompress;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.annotation.SuppressLint;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import com.tbruyelle.rxpermissions2.RxPermissions;

import io.reactivex.functions.Consumer;

public class MainActivity extends AppCompatActivity {

    private ImageView iv;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        iv = findViewById(R.id.iv);
        initPermission();
    }

    public void showOriginal(View view) {
        Bitmap bmp = BitmapFactory.decodeResource(getResources(), R.mipmap.aaa);
        iv.setImageBitmap(bmp);
    }

    public void compressBitmap(View view) {
        Bitmap bmp = BitmapFactory.decodeResource(getResources(), R.mipmap.aaa);
        final String pathFile = Environment.getExternalStorageDirectory().getAbsolutePath() + "/bbb.jpeg";
        BitmapCompressUtils.nativeCompress(bmp,25,pathFile);
    }

    public void showCompressBitmap(View view) {
        final String pathFile =Environment.getExternalStorageDirectory().getAbsolutePath() + "/bbb.jpeg";
        Bitmap bitmapCompress = BitmapFactory.decodeFile(pathFile);
        iv.setImageBitmap(bitmapCompress);
    }



    @SuppressLint("CheckResult")
    private void initPermission() {
        RxPermissions rxPermissions = new RxPermissions(this);
        rxPermissions
                .request(Manifest.permission.READ_EXTERNAL_STORAGE,Manifest.permission.WRITE_EXTERNAL_STORAGE)
                .subscribe(new Consumer<Boolean>() {
                    @Override
                    public void accept(Boolean granted) throws Exception {

                    }
                });
    }
}
