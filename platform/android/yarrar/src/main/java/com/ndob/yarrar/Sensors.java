package com.ndob.yarrar;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;

public class Sensors implements SensorEventListener
{
    private YarrarActivity mParent;
    private SensorManager mSensorManager;
    private int mAccuracy;
    private boolean mUpdating;

    public Sensors(YarrarActivity parent) {
        mParent = parent;
        mSensorManager = (SensorManager) parent.getSystemService(Context.SENSOR_SERVICE);
        mAccuracy = Integer.MAX_VALUE;
        mUpdating = false;
    }

    public void setUpdating(boolean shouldUpdate) {
        if (!mUpdating && shouldUpdate) {
            mUpdating = true;
            // TYPE_ROTATION_VECTOR is a virtual fused sensor.
            // It uses acc-, gyro- and mag-sensors, if available.
            mSensorManager.registerListener(
                this,
                mSensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR),
                SensorManager.SENSOR_DELAY_UI);
        } 
        else if(mUpdating && !shouldUpdate) {
            mUpdating = false;
            mSensorManager.unregisterListener(this);
        }
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        if(event.sensor.getType() == Sensor.TYPE_ROTATION_VECTOR) {
            mAccuracy = event.accuracy;
            float[] quaternion = new float[4];
            SensorManager.getQuaternionFromVector(quaternion, event.values);

            mParent.onSensorRotationUpdate(quaternion);
            Log.d("yarrar", "Orientation quaternion: " + 
                quaternion[0] + ", " + 
                quaternion[1] + ", " + 
                quaternion[2] + ", " + 
                quaternion[3]);
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        mAccuracy = accuracy;
    }
}
