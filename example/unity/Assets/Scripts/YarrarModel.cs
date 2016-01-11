using UnityEngine;
using System.Collections;

public class YarrarModel : MonoBehaviour 
{
	private GameObject m_pipelineManager;
	public int coordinateSystemId;

	// Use this for initialization
	void Start()
	{
		GameObject yarrarObj = GameObject.Find("yarrar");
		if(yarrarObj == null)
		{
			throw new MissingReferenceException("Missing yarrar-object");
		}

		m_pipelineManager = yarrarObj.transform.FindChild("PipelineManager").gameObject;
		if(m_pipelineManager == null)
		{
			throw new MissingReferenceException("Missing yarrar-PipelineManager-object");
		}

		m_pipelineManager.GetComponent<PipelineManager>().RegisterModel(coordinateSystemId, gameObject);
	}

	// Update is called once per frame
	void Update() 
	{
	
	}
}
