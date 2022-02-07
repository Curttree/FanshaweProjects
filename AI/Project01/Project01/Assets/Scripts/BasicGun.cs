using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BasicGun : MonoBehaviour
{
    [SerializeField] private float range = 200f;
    [SerializeField] private Camera fpsCamera;

    private Vector3 destination;

    [SerializeField] private GameObject paintballPrefab;
    [SerializeField] private Transform spawnPoint;
    [SerializeField] private float rateOfFire;

    private float fireTime;

    private void Start()
    {
        fireTime = 0f;
    }
    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButton(0) && rateOfFire < fireTime)
        {
            Fire();
            fireTime = 0f;
        }
        fireTime += Time.deltaTime;
    }

    void Fire()
    {
        Ray ray = fpsCamera.ViewportPointToRay(new Vector3(0.5f, 0.5f, 0.0f));
        destination = ray.GetPoint(range);
        GameObject projectile = Instantiate(paintballPrefab, spawnPoint.position, Quaternion.identity);
        projectile.GetComponent<Rigidbody>().velocity = (destination - spawnPoint.position);
    }
}
