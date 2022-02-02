using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerMovement : MonoBehaviour
{
    public CharacterController controller;
    public float speed = 12.0f;

    public float gravity = -9.81f;
    public Vector3 velocity;

    public float jumpHeight = 3.0f;

    // Update is called once per frame
    void Update()
    {
        float x = Input.GetAxis("Horizontal");
        float z = Input.GetAxis("Vertical");
        Vector3 move = (transform.right * x) + (transform.forward * z);
        controller.Move(move * speed * Time.deltaTime);

        if (controller.isGrounded)
        {
            velocity.y = 0.0f;
        }

        if (Input.GetButtonDown("Jump"))
        {
            Debug.Log("Space bar is pressed");
            velocity.y = Mathf.Sqrt(-2.0f * jumpHeight * gravity);
        }

        velocity.y += gravity * Time.deltaTime;
        controller.Move(velocity * Time.deltaTime);
    }
}
