using UnityEngine;
using System.Collections;

public class BasketTriggers : MonoBehaviour 
{
    private bool _topHit;
    private float _timeHit;
    private AudioSource _audioSource;

    void Start () 
	{
        _audioSource = GetComponent<AudioSource>();
	}
	
	void Update () 
	{

	}

    public void TopTriggerHit()
    {
        _topHit = true;
        _timeHit = Time.realtimeSinceStartup;
    }

    public void BottomTriggerHit()
    {
        if (_topHit && Time.realtimeSinceStartup - _timeHit < 4)
        {
            _topHit = false;
            _timeHit = 0;
            _audioSource.Play();
        }

    }
}
