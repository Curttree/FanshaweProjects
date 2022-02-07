using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TypeBBehaviour : EnemyBehaviour
{
    [SerializeField] private Pursue pursue;
    [SerializeField] private Evade evade;
    [SerializeField] private Material pursueMaterial;
    [SerializeField] private Material evadeMaterial;

    private void Start()
    {
        pursue.target = target;
    }
    private void Update()
    {
        Transform nearestBullet = FindNearestBullet();
        if (nearestBullet)
        {
            evade.enabled = true;
            evade.target = nearestBullet;
            gameObject.GetComponent<MeshRenderer>().material = evadeMaterial;
        }
        else
        {
            evade.enabled = false;
            gameObject.GetComponent<MeshRenderer>().material = pursueMaterial;
        }
    }

    Transform FindNearestBullet()
    {
        Transform nearest = null;
        GameObject[] bullets = GameObject.FindGameObjectsWithTag("Bullet");
        foreach(GameObject bullet in bullets) {
            if (nearest)
            {
                if (Vector3.Distance(bullet.transform.position, transform.position) < Vector3.Distance(nearest.transform.position, transform.position))
                {
                    nearest = bullet.transform;
                }
            }
            else
            {
                // No nearest value has been set. This is therefore the nearest.
                nearest = bullet.transform;
            }
        }
        return nearest;
    }
}
