package com.ndob.yarrar;

import android.content.res.AssetManager;
import android.hardware.Camera;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.LinearLayout;

public class YarrarActivity extends ActionBarActivity implements Camera.PreviewCallback {
    private static final String TAG = "YarrarActivity";
    private Camera mCamera;
    private AssetManager mAssetManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mAssetManager = getResources().getAssets();

        mCamera = getCameraInstance();
        mCamera.setPreviewCallback(this);
        mCamera.startPreview();

        System.loadLibrary("yarrar");
        Log.i(TAG, "Yarrar glue loaded.");
    }

    @Override
    protected void onDestroy() {
        deinitYarrar();
        super.onDestroy();
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
        Camera.Size size = mCamera.getParameters().getPreviewSize();
        injectCameraFrame(size.width, size.height, data);
    }

    public void onOpenGLSurfaceCreated() {
        Camera.Size size = mCamera.getParameters().getPreviewSize();
        initYarrar(size.width, size.height, mAssetManager);
        onYarrarInitialized();
    }

    public void onOpenGLDrawFrame() {
        run();
    }

    protected void initialize(LinearLayout layout) {
        layout.addView(new OpenGLSurface(this));
    }

    public void onYarrarInitialized() {
    }

    private native void initYarrar(int width, int height, AssetManager assetManager);
    private native void deinitYarrar();
    private native void run();
    private native void injectCameraFrame(int width, int height, byte[] cameraData);
    protected native void addModel(float[] vertices);
}
