using UnityEngine;
using System.Collections;

public class ArmController : MonoBehaviour 
{
    public QuadleController ParentQuadleController;

    void OnTriggerEnter2D(Collider2D other)
    {
        if (other.gameObject.tag == "Quadle")
        {
            other.gameObject.GetComponent<QuadleController>().OnArmCollision(ParentQuadleController);
        }
    }
}
