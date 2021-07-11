using UnityEngine;
using System.Collections;

public class TopTrigger : MonoBehaviour 
{
    public BasketTriggers Parent;

    void OnTriggerEnter(Collider collider)
    {
        Parent.TopTriggerHit();
    }
}
