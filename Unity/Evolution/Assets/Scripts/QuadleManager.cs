using UnityEngine;
using System.Collections;

public class QuadleManager : MonoBehaviour 
{
    public int QuadleStartCount;
    public GameObject QuadleObject;

	void Start() 
	{
        AddQuadles();
	}
	
	void Update() 
	{
	
	}

    void AddQuadles()
    {
        GameObject quadles = GameObject.FindGameObjectWithTag("Quadles");

        for (int i = 0; i < QuadleStartCount; i++)
        {
            GameObject quadle = Instantiate(QuadleObject);
            quadle.transform.parent = quadles.transform;
            quadle.GetComponent<Rigidbody2D>().position = new Vector2(Random.Range(-9f, 9f), Random.Range(-6f, 6f));
        }
    }

    void OnGUI()
    {
        Time.timeScale = GUI.VerticalSlider(new Rect(20, 20, 20, 100), Time.timeScale, 5.0f, 0.5f);
    }
}
