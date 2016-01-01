using UnityEngine;
using UnityEngine.UI;
using System;
using System.Collections;
using System.Runtime.InteropServices;

public class WebcamTexture : MonoBehaviour 
{
    [DllImport ("yarrar")]
    private static extern IntPtr getPose(IntPtr pixelBuffer, int width, int height);    

    private struct Pose
    {
        public bool valid;
        public float[] rotation;
        public float[] translation;
        public float[] camera;
    }

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
        var pose = JsonUtility.FromJson<Pose>(jsonString);

        m_cube.SetActive(pose.valid);

        if(pose.valid)
        {
            var camera = m_camera.GetComponent<Camera>();

            Matrix4x4 viewMatrix = Matrix4x4.identity;
            // Rotation
            viewMatrix.m00 = pose.rotation[0];
            viewMatrix.m01 = pose.rotation[1];
            viewMatrix.m02 = pose.rotation[2];
            viewMatrix.m10 = pose.rotation[3];
            viewMatrix.m11 = pose.rotation[4];
            viewMatrix.m12 = pose.rotation[5];
            viewMatrix.m20 = pose.rotation[6];
            viewMatrix.m21 = pose.rotation[7];
            viewMatrix.m22 = pose.rotation[8];
            // Translation
            viewMatrix.m03 = pose.translation[0];
            viewMatrix.m13 = pose.translation[1];
            viewMatrix.m23 = pose.translation[2];

            // Invert y and z (stored as inverted in OpenCV).
            Matrix4x4 conversionMatrix = Matrix4x4.identity;
            conversionMatrix.m00 = 1.0f;
            conversionMatrix.m11 = -1.0f;
            conversionMatrix.m22 = -1.0f;
            conversionMatrix.m33 = 1.0f;

            camera.worldToCameraMatrix = conversionMatrix * viewMatrix;

            float fx = pose.camera[0];
            float cx = pose.camera[2];
            float fy = pose.camera[4];
            float cy = pose.camera[5];
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
