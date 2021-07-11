using UnityEngine;
using System.Collections;

public class CameraCollider : MonoBehaviour 
{
    public Camera TileCamera;

	void Start() 
	{
	
	}

	void Update() 
	{
	
	}

    void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.layer == 8)
        {
            TileCamera.enabled = true;
        }
    }

    void OnTriggerStay(Collider other)
    {

    }

    void OnTriggerExit(Collider other)
    {
        if (other.gameObject.layer == 8)
        {
            TileCamera.enabled = false;
        }
    }
}
