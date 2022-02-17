using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Vehicle : MonoBehaviour
{
    public Seek _seek;
    // Start is called before the first frame update
    void Start()
    {
        Seek(transform.position);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void Seek(Vector3 newTarget)
    {
        _seek.target = newTarget;
        //transform.LookAt(newTarget);
    }
}
