using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerBehaviour : MonoBehaviour
{
    [SerializeField] private Vector3 spawnPosition;
    [SerializeField] private CharacterController charController;
    [SerializeField] private GameObject afterlifePanel;

    public void Respawn()
    {
        charController.enabled = false;
        transform.position = spawnPosition;
        charController.enabled = true;
        afterlifePanel.GetComponent<FlashUIPanel>().Flash();
    }

    public void OnControllerColliderHit(ControllerColliderHit hit)
    {

        if (hit.gameObject.CompareTag("Enemy"))
        {
            Respawn();
            Destroy(hit.gameObject);
        }
    }
}
