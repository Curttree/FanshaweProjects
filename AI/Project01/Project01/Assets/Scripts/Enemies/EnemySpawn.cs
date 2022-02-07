using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemySpawn : MonoBehaviour
{
    [SerializeField] private List<GameObject> enemies;
    [SerializeField] private float spawnTime;
    [SerializeField] private Transform player;
    private float timeToSpawn;
    private Vector3 newScale;
    // Start is called before the first frame update
    void Start()
    {
        timeToSpawn = 0f;
    }

    // Update is called once per frame
    void Update()
    {
        newScale = transform.localScale;
        newScale.x += Time.deltaTime * (0.25f/spawnTime);
        newScale.z += Time.deltaTime * (0.25f / spawnTime);
        transform.localScale = newScale;
        timeToSpawn += Time.deltaTime;
        if (timeToSpawn > spawnTime)
        {
            if (enemies.Count > 0)
            {
                int selection = Random.Range(0, enemies.Count);
                GameObject enemy = Instantiate(enemies[selection], transform.position, Quaternion.identity);
                enemy.GetComponent<EnemyBehaviour>().target = player;
            }
            transform.localScale = Vector3.zero;
            timeToSpawn = 0f;
        }
    }
}
