using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class FlockingOutput : MonoBehaviour
{
    public Flocking referenceFlocking;
    public Text outputText;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        outputText.text = $"Flocking Values || Cohesion: {referenceFlocking.cohWeight}, Separation: {referenceFlocking.sepWeight}, Alignment: {referenceFlocking.aliWeight}";
    }
}
