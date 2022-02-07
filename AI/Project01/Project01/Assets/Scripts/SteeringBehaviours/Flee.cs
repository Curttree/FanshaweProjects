using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Flee : SteeringBehaviour
{
    public override Vector3 CalculateSteeringForce()
    {
        /*calculate the direction*/
        /*Flee use current position - target position*/
        Vector3 direction = transform.position - target.position;

        Vector3 desiredVelocity = direction.normalized * maxSpeed;

        Vector3 steer = Vector3.ClampMagnitude(desiredVelocity - rb.velocity, maxForce);

        steer = steer / mass;

        return steer;
    }
}
