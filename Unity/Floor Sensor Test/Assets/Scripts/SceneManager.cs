using UnityEngine;
using System.Collections;

public class SceneManager : MonoBehaviour 
{
	void Start() 
	{

	}

	void Update() 
	{
        if (Input.GetKeyDown(KeyCode.Escape) || Input.GetButtonDown("Back"))
        {
            Application.Quit();
        }
	}
}
