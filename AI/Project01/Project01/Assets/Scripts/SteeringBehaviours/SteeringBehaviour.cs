using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SteeringBehaviour : MonoBehaviour
{
    public Transform target;
    public float weight = 1.0f;
    public float mass = 1.0f;

    public float maxSpeed = 1.0f;
    public float maxForce = 1.0f;
    public float maxTurnSpeed = 1.0f;

    public Rigidbody rb;

    // Start is called before the first frame update
    void Start()
    {
        rb = GetComponent<Rigidbody>();
        if(rb == null)
        {
            gameObject.AddComponent<Rigidbody>();
            rb = GetComponent<Rigidbody>();
        }
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 steer = CalculateSteeringForce();
        ApplySteer(steer);
        LookWhereYoureGoing();
    }

    public virtual Vector3 CalculateSteeringForce()
    {
        return Vector3.zero;
    }

    public void ApplySteer(Vector3 accel)
    {
        rb.velocity += accel * Time.deltaTime * weight;

        if( rb.velocity.magnitude > maxSpeed)
        {
            rb.velocity = rb.velocity.normalized * maxSpeed;
        }
    }

    public void LookWhereYoureGoing()
    {
        Vector3 direction = rb.velocity;

        LookAtDirection(direction);
    }

    public void LookAtDirection(Vector3 direction)
    {
        direction.Normalize();
        /*If we have a non-zero direction then look towards that direction, otherwise do noting*/
        if(direction.sqrMagnitude > 0.001f)
        {
            /*Determine how the agent should rotate to face the direction on the Y axis*/
            /*Atan2 returns the angle in radians whose tan is direction.z/direction.x */
            float rotY = -1 * Mathf.Atan2(direction.z, direction.x) * Mathf.Rad2Deg;
            /*Determines the amount to turn from it's current rotation to it's desired rotation based on max turn speed */
            float rotationAmount = Mathf.LerpAngle(transform.rotation.eulerAngles.y, rotY, Time.deltaTime * maxTurnSpeed);
            /*Converts angle to Quaterion and applies rotation to agent*/
            transform.rotation = Quaternion.Euler(0, rotationAmount, 0);

        }
    }
}
