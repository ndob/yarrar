package com.ndob.yarrar;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;

import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class Sensors implements SensorEventListener
{
    private YarrarActivity mParent;
    private SensorManager mSensorManager;
    private int mAccuracy;
    private boolean mUpdating;
    private Set<Integer> mActiveSensors;

    public Sensors(YarrarActivity parent) {
        mParent = parent;
        mSensorManager = (SensorManager) parent.getSystemService(Context.SENSOR_SERVICE);
        mAccuracy = Integer.MAX_VALUE;
        mUpdating = false;
        mActiveSensors = new HashSet<Integer>();
    }

    public boolean setSensorState(int sensorType, boolean active) {
        List<Sensor> available = mSensorManager.getSensorList(Sensor.TYPE_ALL);

        boolean found = false;
        for(Sensor s : available) {
            if(s.getType() == sensorType) {
                found = true;
                break;
            }
        }

        if(!found) {
            return false;
        }

        if(active) {
            mActiveSensors.add(sensorType);
        }
        else {
            mActiveSensors.remove(sensorType);
        }

        refreshListeners();

        return true;
    }

    public void setUpdating(boolean shouldUpdate) {
        if (!mUpdating && shouldUpdate) {
            mUpdating = true;
            refreshListeners();
        } 
        else if(mUpdating && !shouldUpdate) {
            mUpdating = false;
            mSensorManager.unregisterListener(this);
        }
    }

    private void refreshListeners() {
        if(mActiveSensors.isEmpty()) {
            mSensorManager.unregisterListener(this);
        }
        else {
            for(int sensorType : mActiveSensors) {
                mSensorManager.registerListener(
                    this,
                    mSensorManager.getDefaultSensor(sensorType),
                    SensorManager.SENSOR_DELAY_UI);
            }
        }
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        if(event.sensor.getType() == Sensor.TYPE_ROTATION_VECTOR) {
            float[] quaternion = new float[4];
            SensorManager.getQuaternionFromVector(quaternion, event.values);
            mParent.onSensorRotationUpdate(quaternion);
        }
        else if(event.sensor.getType() == Sensor.TYPE_GYROSCOPE) {
            mParent.onSensorGyroscopeUpdate(event.values);
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
    }
}
