using UnityEngine;
using UnityEngine.UI;
using System;
using System.Collections;
using System.Runtime.InteropServices;
using SimpleJSON;

public class WebcamTexture : MonoBehaviour 
{
    [DllImport ("yarrar")]
    private static extern IntPtr getPose(IntPtr pixelBuffer, int width, int height);    

    private WebCamTexture m_webcamTexture;
    public RawImage rawimage;
    public GameObject m_camera;
    public GameObject m_cube;

    void Start()
    {
        m_webcamTexture = new WebCamTexture();
        rawimage.texture = m_webcamTexture;
        rawimage.material.mainTexture = m_webcamTexture;
        m_webcamTexture.Play();
    }

    void Update() 
    {
        Color32[] pixelData = m_webcamTexture.GetPixels32();
        IntPtr poseDataStringPtr;
        GCHandle handle = GCHandle.Alloc(pixelData, GCHandleType.Pinned);
        try
        {
            poseDataStringPtr = getPose(handle.AddrOfPinnedObject(), m_webcamTexture.width, m_webcamTexture.height);
        }
        finally
        {
            handle.Free();
        }

        var jsonString = Marshal.PtrToStringAnsi(poseDataStringPtr);
        var poseData = JSON.Parse(jsonString);

        bool valid = poseData["valid"].AsInt != 0 ? true : false;
        m_cube.SetActive(valid);

        if(valid)
        {
            var camera = m_camera.GetComponent<Camera>();
            var translation = poseData["translation"].AsArray;
            var rotationMtx = poseData["rotation"].AsArray;
            var cameraMatrix = poseData["camera"].AsArray;

            Matrix4x4 viewMatrix = Matrix4x4.identity;
            // Rotation
            viewMatrix.m00 = rotationMtx[0].AsFloat;
            viewMatrix.m01 = rotationMtx[1].AsFloat;
            viewMatrix.m02 = rotationMtx[2].AsFloat;
            viewMatrix.m10 = rotationMtx[3].AsFloat;
            viewMatrix.m11 = rotationMtx[4].AsFloat;
            viewMatrix.m12 = rotationMtx[5].AsFloat;
            viewMatrix.m20 = rotationMtx[6].AsFloat;
            viewMatrix.m21 = rotationMtx[7].AsFloat;
            viewMatrix.m22 = rotationMtx[8].AsFloat;

            // Translation
            viewMatrix.m03 = translation[0].AsFloat;
            viewMatrix.m13 = translation[1].AsFloat;
            viewMatrix.m23 = translation[2].AsFloat;

            // Invert y and z (stored as inverted in OpenCV).
            Matrix4x4 conversionMatrix = Matrix4x4.identity;
            conversionMatrix.m00 = 1.0f;
            conversionMatrix.m11 = -1.0f;
            conversionMatrix.m22 = -1.0f;
            conversionMatrix.m33 = 1.0f;

            camera.worldToCameraMatrix = conversionMatrix * viewMatrix;

            float fx = cameraMatrix[0].AsFloat;
            float cx = cameraMatrix[2].AsFloat;
            float fy = cameraMatrix[4].AsFloat;
            float cy = cameraMatrix[5].AsFloat;
            float far = 10.0f;
            float near = 0.1f;

            Matrix4x4 projection = Matrix4x4.identity;
            projection.m00 = fx / cx;
            projection.m11 = fy / cy;
            projection.m22 = (-1.0f * (far + near) / (far - near));
            projection.m23 = (-2.0f * far * near) / (far - near);
            projection.m32 = -1;
            projection.m33 = 0;

            camera.projectionMatrix = projection;
        }
    }
}
