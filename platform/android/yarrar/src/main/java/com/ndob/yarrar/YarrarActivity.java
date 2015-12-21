package com.ndob.yarrar;


import android.hardware.Camera;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.LinearLayout;

public class YarrarActivity extends ActionBarActivity implements Camera.PreviewCallback {
    private static final String TAG = "YARRAR::AndroidExample";
    private Camera mCamera;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mCamera = getCameraInstance();
        mCamera.setPreviewCallback(this);
        mCamera.startPreview();

        System.loadLibrary("yarrar");
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
    }

    protected void initialize(LinearLayout layout) {
        layout.addView(new OpenGLSurface(this));
    }

    public void onYarrarInitialized() {
    }

    public native void initYarrar();
    public native void run();
    protected native void addModel(float[] vertices);
    private native void injectCameraFrame(int width, int height, byte[] cameraData);
}
