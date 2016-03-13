package com.ndob.yarrar;

import android.app.AlertDialog;
import android.content.res.AssetManager;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.LinearLayout;

import java.io.IOException;
import java.lang.Override;

public class YarrarActivity extends ActionBarActivity implements Camera.PreviewCallback {
    private static final String TAG = "YarrarActivity";
    private static final int DUMMY_TEXTURE_ID = 10;

    private Camera mCamera;
    // TODO: Use mCameraTexture with OpenGL-rendering, instead of uploading the texture every frame.
    // For the moment this is only a dummy texture, because otherwise the preview doesn't start.
    private SurfaceTexture mCameraTexture;
    private AssetManager mAssetManager;
    private Sensors mSensors;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        System.loadLibrary("yarrar");
        Log.i(TAG, "Yarrar glue loaded.");

        mSensors = new Sensors(this);
        mAssetManager = getResources().getAssets();
        mCamera = getCameraInstance();
        if(mCamera == null) {
            showError("Can't open camera.");
            return;
        }
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
        if (mSensors != null) {
            mSensors.setUpdating(false);
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        if (mSensors != null) {
            mSensors.setUpdating(true);
        }
    }

    @Override
    public void onPreviewFrame(byte[] data, Camera camera) {
        Camera.Size size = mCamera.getParameters().getPreviewSize();
        injectCameraFrame(size.width, size.height, data);
    }

    public void onOpenGLSurfaceCreated() {
        try {
            mCameraTexture = new SurfaceTexture(DUMMY_TEXTURE_ID);
            mCamera.setPreviewTexture(mCameraTexture);
        } catch (IOException e) {
            e.printStackTrace();
        }

        mCamera.setPreviewCallback(this);
        mCamera.startPreview();

        Camera.Size size = mCamera.getParameters().getPreviewSize();
        initYarrar(size.width, size.height, mAssetManager);
        onYarrarInitialized();
    }

    public void onOpenGLDrawFrame() {
        run();
    }

    public void onSensorRotationUpdate(float[] quaternion) {
        injectSensorRotation(quaternion);
    }

    protected void initialize(LinearLayout layout) {
        layout.addView(new OpenGLSurface(this));
    }

    public void onYarrarInitialized() {
    }

    private void showError(String msg) {
        AlertDialog.Builder alert = new AlertDialog.Builder(this);
        alert.setTitle("Error");
        alert.setMessage(msg);
        alert.setPositiveButton("OK", null);
        alert.show();
    }

    private native void initYarrar(int width, int height, AssetManager assetManager);
    private native void deinitYarrar();
    private native void run();
    private native void injectCameraFrame(int width, int height, byte[] cameraData);
    private native void injectSensorRotation(float[] rotationQuaternion);
    protected native void addModel(int coordinateSystemId, float[] vertices);
}
