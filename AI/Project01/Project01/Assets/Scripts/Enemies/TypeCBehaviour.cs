using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TypeCBehaviour : EnemyBehaviour
{
    [SerializeField] private Approach approach;
    [SerializeField] private Material approachMaterial;
    [SerializeField] private Material fireMaterial;
    [SerializeField] private GameObject paintballPrefab;

    private bool readyToFire;
    public float timeToFire;
    private float timeWaiting;
    private Vector3 destination;
    private Vector3 spawnPosition;

    private void Start()
    {
        approach.target = target;
    }
    private void Update()
    {
        if (approach.insideStoppingRadius)
        {
            transform.LookAt(target);
            if (!readyToFire)
            {
                gameObject.GetComponent<MeshRenderer>().material = fireMaterial;
                readyToFire = true;
            }
            timeWaiting += Time.deltaTime;
            if (timeWaiting >= timeToFire)
            {
                Fire();
                timeWaiting = 0f;
            }
        }
        else if (!approach.insideStoppingRadius && readyToFire)
        {
            gameObject.GetComponent<MeshRenderer>().material = approachMaterial;
            readyToFire = false;
            timeWaiting = 0;
        }
    }
    void Fire()
    {
        destination = target.position;
        spawnPosition = transform.position + transform.forward.normalized * 0.7f;
        GameObject projectile = Instantiate(paintballPrefab, spawnPosition, Quaternion.identity);
        projectile.GetComponent<Rigidbody>().velocity = (destination - spawnPosition);
    }
}
