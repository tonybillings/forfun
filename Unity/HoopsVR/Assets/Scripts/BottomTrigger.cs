using UnityEngine;
using System.Collections;

public class BottomTrigger : MonoBehaviour 
{
    public BasketTriggers Parent;

    void OnTriggerEnter(Collider collider)
    {
        Parent.BottomTriggerHit();
    }
}
