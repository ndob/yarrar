using UnityEngine;
using UnityEngine.UI;
using System;
using System.Collections;

public class CameraDataprovider : MonoBehaviour 
{
	private WebCamTexture m_webcamTexture;
    public WebCamTexture CameraTexture
    {
		get { return m_webcamTexture; }
    }

    void Awake()
    {
        m_webcamTexture = new WebCamTexture();
        var rawImage = GetComponent<RawImage>();
		rawImage.texture = m_webcamTexture;
		rawImage.material.mainTexture = m_webcamTexture;
        m_webcamTexture.Play();
    }
}
