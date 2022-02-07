using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TypeDBehaviour : EnemyBehaviour
{
    [SerializeField] private Wander wander;
    [SerializeField] private Material wanderMaterial;
    [SerializeField] private Material idleMaterial;
    [SerializeField] private Rigidbody rb;

    private float timer;

    private float idleTime = 3f;
    private float wanderTime = 6f;
    private bool isWandering = true;

    private void Start()
    {
        wander.target = target;
        wander.enabled = true;
    }

    private void Update()
    {
        timer += Time.deltaTime;
        if (isWandering && timer >= wanderTime)
        {
            timer = 0;
            wander.enabled = false;
            isWandering = false;
            gameObject.GetComponent<MeshRenderer>().material = idleMaterial;
        }
        if (!isWandering && timer >= idleTime)
        {
            timer = 0;
            wander.enabled = true;
            isWandering = true;
            gameObject.GetComponent<MeshRenderer>().material = wanderMaterial;
        }
        if (!isWandering)
        {
            rb.velocity = new Vector3(0f, 0f, 0f);
        }
    }
}
