using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyPaintball : MonoBehaviour
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
        if (collision.gameObject.CompareTag("Player"))
        {
            collision.gameObject.GetComponent<PlayerBehaviour>().Respawn();
        }

        if (!collision.gameObject.CompareTag("Enemy"))
        {
            Destroy(gameObject);
        }
    }
}
