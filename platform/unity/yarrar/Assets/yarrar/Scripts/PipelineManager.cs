using UnityEngine;
using UnityEngine.UI;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class PipelineManager : MonoBehaviour
{
#if UNITY_STANDALONE_WIN || UNITY_EDITOR_WIN 
    [DllImport ("yarrar")]
    private static extern IntPtr initYarrar(int width, int height);
    [DllImport ("yarrar")]
    private static extern IntPtr deinitYarrar();
    [DllImport ("yarrar")]
    private static extern IntPtr getPose(IntPtr pixelBuffer, int width, int height);
#else
    [DllImport ("libyarrar")]
    private static extern IntPtr initYarrar(int width, int height);
    [DllImport ("libyarrar")]
    private static extern IntPtr deinitYarrar();
    [DllImport ("libyarrar")]
    private static extern IntPtr getPose(IntPtr pixelBuffer, int width, int height);
#endif

    [Serializable]
    private struct Pose
    {
        public int coordinateSystemId;
        public float[] rotation;
        public float[] translation;
        public float[] camera;
    }

    [Serializable]
    private struct PoseCollection
    {
        public Pose[] poses;
    }

    private struct CoordinateSystem
    {
        public List<GameObject> objects;
        public GameObject camera;
        public int cullingMask;
        public int layerNumber;
    }

    public GameObject m_backgroundCanvas;
    public GameObject m_cameraPrefab;

    private Dictionary<int, CoordinateSystem> m_coordinateSystems = new Dictionary<int, CoordinateSystem>();
    private WebCamTexture m_webcamTexture;

    void Start()
    {
        m_webcamTexture = new WebCamTexture();
        var rawImage = m_backgroundCanvas.GetComponent<RawImage>();
        rawImage.texture = m_webcamTexture;
        rawImage.material.mainTexture = m_webcamTexture;
        rawImage.rectTransform.sizeDelta = new Vector2(Screen.width, Screen.height);
        m_webcamTexture.Play();

        initYarrar(m_webcamTexture.width, m_webcamTexture.height);
        Debug.Log("Yarrar initialized.");
    }

    void OnDestroy() 
    {
        deinitYarrar();
        Debug.Log("Yarrar deinitialized.");
    }

    void Update() 
    {
        Color32[] pixelData = m_webcamTexture.GetPixels32();
        IntPtr poseDataStringPtr;
        GCHandle handle = GCHandle.Alloc(pixelData, GCHandleType.Pinned);
        try
        {
            poseDataStringPtr = getPose(handle.AddrOfPinnedObject(), 
                m_webcamTexture.width, 
                m_webcamTexture.height);
        }
        finally
        {
            handle.Free();
        }

        var jsonString = Marshal.PtrToStringAnsi(poseDataStringPtr);
        if(jsonString.Length == 0)
        {
            foreach(var key in m_coordinateSystems.Keys)
            {
                SetCoordinateSystemActive(key, false);
            }
            return;
        }

        var poseCollection = JsonUtility.FromJson<PoseCollection>(jsonString);
        for(int i = 0; i < poseCollection.poses.Length; ++i)
        {
            UpdateCoordinateSystem(poseCollection.poses[i]);
        }        
    }

    void SetCoordinateSystemActive(int id, bool active)
    {
        if(m_coordinateSystems.ContainsKey(id))
        {
            var objects = m_coordinateSystems[id].objects;
            for(int i = 0; i < objects.Count; ++i)
            {
                objects[i].SetActive(active);
            }
        }
    }

    void UpdateCoordinateSystem(Pose pose)
    {
        if(!m_coordinateSystems.ContainsKey(pose.coordinateSystemId))
        {
            return;
        }

        SetCoordinateSystemActive(pose.coordinateSystemId, true);
        var camera = m_coordinateSystems[pose.coordinateSystemId].camera.GetComponent<Camera>();

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

    public void RegisterModel(int coordinateSystemId, GameObject go)
    {
        if(!m_coordinateSystems.ContainsKey(coordinateSystemId))
        {
            CreateNewCoordinateSystem(coordinateSystemId);
        }

        SetLayer(go, m_coordinateSystems[coordinateSystemId].layerNumber, true);
        m_coordinateSystems[coordinateSystemId].objects.Add(go);
    }

    private void CreateNewCoordinateSystem(int id)
    {
        if((31 - m_coordinateSystems.Count) < 15)
        {
            throw new OverflowException("Too many coordinate systems in use.");
        }

        var coordSystem = new CoordinateSystem();
        coordSystem.objects = new List<GameObject>();
        coordSystem.layerNumber = (31 - m_coordinateSystems.Count);
        coordSystem.cullingMask =  1 << coordSystem.layerNumber;

        coordSystem.camera = Instantiate(m_cameraPrefab);
        // Add new camera under "Cameras"-entity.
        coordSystem.camera.transform.parent = transform.parent.FindChild("Cameras");
        coordSystem.camera.GetComponent<Camera>().cullingMask = coordSystem.cullingMask;
        m_coordinateSystems[id] = coordSystem;
    }

    private void SetLayer(GameObject go, int layer, bool recursive)
    {
        go.layer = layer;
        if(recursive)
        {
            for(int i = 0; i < go.transform.childCount; ++i)
            {
                SetLayer(go.transform.GetChild(i).gameObject, layer, true);
            }
        }
    }
}
