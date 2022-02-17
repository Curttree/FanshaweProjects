using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Seek : SteeringBehaviour
{
    public float arriveRadius = 5f;
    public override Vector3 CalculateSteeringForce()
    {
        /*calculate the direction*/
        /*Seek use target - current position*/
        // Modified so targets a vector3 rather than an object's position.
        Vector3 direction = target - transform.position;

        if (direction.magnitude <= arriveRadius / 2f)
        {
            // They've arrived. Don't bother with the rest of the calculations, just stop them.
            return -rb.velocity;
        }

        Vector3 desiredVelocity = direction.normalized * maxSpeed;

        /*calculate distance*/
        float distance = direction.magnitude;
        /*Arrive steering behaviour*/
        if(distance < arriveRadius)
        {
            desiredVelocity *= ((distance-0.75f) / arriveRadius);
            //Debug.Log(desiredVelocity);
        }

        Vector3 steer = Vector3.ClampMagnitude(desiredVelocity - rb.velocity, maxForce);

        steer = steer / mass;

        return steer;
    }
}
