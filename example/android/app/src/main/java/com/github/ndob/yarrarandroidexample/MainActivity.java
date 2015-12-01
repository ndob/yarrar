package com.github.ndob.yarrarandroidexample;

import android.hardware.Camera;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.LinearLayout;

import java.io.File;
import java.io.IOException;

public class MainActivity extends ActionBarActivity implements Camera.PreviewCallback {
    private static final String TAG = "YARRAR::AndroidExample";
    private Camera mCamera;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        LinearLayout layout = (LinearLayout) findViewById(R.id.main_view);
        layout.addView(new OpenGLSurface(this));

        mCamera = getCameraInstance();
        mCamera.setDisplayOrientation(90);
        mCamera.setPreviewCallback(this);
        mCamera.startPreview();

        System.loadLibrary("yarrar_glue");
        Log.i(TAG, "Yarrar glue loaded.");
    }

    public static Camera getCameraInstance() {
        Camera c = null;
        try {
            c = Camera.open();
        } catch (Exception e) {
            Log.i(TAG, "error:" + e.getMessage());
        }
        return c;
    }

    @Override
    public void onPause() {
        super.onPause();
        if (mCamera != null) {
            mCamera.stopPreview();
            mCamera.setPreviewCallback(null);
            mCamera.release();
        }
        mCamera = null;
    }

    @Override
    public void onPreviewFrame(byte[] data, Camera camera) {

        injectCameraFrame(camera.getParameters().getPreviewSize().width, camera.getParameters().getPreviewSize().height, data);
        run();
    }

    public native int initYarrar();
    public native int run();
    public native int runRender();
    public native void injectCameraFrame(int width, int height, byte[] cameraData);

}
