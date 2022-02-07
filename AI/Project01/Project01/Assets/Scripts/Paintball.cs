using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Paintball : MonoBehaviour
{
    private float lifeSpan = 1.5f;
    private float deathClock;

    // Start is called before the first frame update
    void Start()
    {
        deathClock = 0f;
    }

    // Update is called once per frame
    void Update()
    {
        if (deathClock > lifeSpan)
        {
            Destroy(gameObject);
        }
        else
        {
            deathClock += Time.deltaTime;
        }
    }

    private void OnCollisionEnter(Collision collision)
    {
        if (!collision.gameObject.CompareTag("Player"))
        {
            Destroy(gameObject);
        }

        if (collision.gameObject.CompareTag("Enemy"))
        {
            Destroy(collision.gameObject);
        }
    }
}
