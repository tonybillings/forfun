using UnityEngine;
using System.Collections;

public class BallCameraController : MonoBehaviour
{
    private Rigidbody _ballRigidbody;

    void Start()
    {

    }

    void Update()
    {
        if (_ballRigidbody != null)
        {
            transform.position = Vector3.Lerp(transform.position, _ballRigidbody.gameObject.transform.position, 0.2f);
            transform.rotation = Quaternion.Lerp(transform.rotation, Quaternion.LookRotation(Vector3.Normalize(_ballRigidbody.velocity), Vector3.up), 0.2f);
        }
    }

    public void SetBasketBall(ref Rigidbody ballRigidbody)
    {
        _ballRigidbody = ballRigidbody;
    }
}
