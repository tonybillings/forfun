using UnityEngine;
using System.Collections;

public class TileSensor : MonoBehaviour 
{
    public float StepTime = 0.15f;
    public Material BlackMaterial;
    public Material RedMaterial;
    public bool IsActive { get; private set; }
    public Vector2 Location;
    public GameObject TileModel;

    private MeshRenderer _renderer;
    private float _firstCollisionTime;
    private float _lastCollisionTime;
    private bool _isGoingInactive;
    
    private void Awake()
    {
        _renderer = GetComponent<MeshRenderer>();
    }

	private void Start() 
	{
        
	}
	
	private void Update() 
	{
	    if (_isGoingInactive)
        {
            if (Time.timeSinceLevelLoad - _lastCollisionTime > StepTime)
            {
                _renderer.material = BlackMaterial;
                _isGoingInactive = false;
                IsActive = false;
            }
        }
	}

    void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.layer == 8)
        {
            _firstCollisionTime = Time.timeSinceLevelLoad;
            _isGoingInactive = false;
        }
    }

    void OnTriggerStay(Collider other)
    {
        if (other.gameObject.layer == 8 && !IsActive && Time.timeSinceLevelLoad - _firstCollisionTime > StepTime)
        {
            _renderer.material = RedMaterial;
            IsActive = true;
        }
    }

    void OnTriggerExit(Collider other)
    {
        if (other.gameObject.layer == 8)
        {
            _lastCollisionTime = Time.timeSinceLevelLoad;
            _isGoingInactive = true;
        }
    }
}