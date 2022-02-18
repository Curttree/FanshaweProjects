using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FakeVehicle : MonoBehaviour
{
    public Flocking _flocking;

    // Update is called once per frame
    void LateUpdate()
    {
        if (_flocking.enabled)
        {
            transform.position = transform.parent.position;
        }
    }
}
