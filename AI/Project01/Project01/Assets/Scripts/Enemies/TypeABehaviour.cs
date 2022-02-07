using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TypeABehaviour : EnemyBehaviour
{
    [SerializeField] private SteeringBehaviour flee;
    [SerializeField] private SteeringBehaviour seek;
    [SerializeField] private Material fleeMaterial;
    [SerializeField] private Material seekMaterial;

    private void Start()
    {
        flee.target = target;
        seek.target = target;
    }
    private void OnBecameVisible()
    {
        flee.enabled = true;
        seek.enabled = false;
        gameObject.GetComponent<MeshRenderer>().material = fleeMaterial;
    }
    private void OnBecameInvisible()
    {
        flee.enabled = false;
        seek.enabled = true;
        gameObject.GetComponent<MeshRenderer>().material = seekMaterial;
    }
}
