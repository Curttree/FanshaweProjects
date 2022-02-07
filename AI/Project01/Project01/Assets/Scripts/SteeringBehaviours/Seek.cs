using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Seek : SteeringBehaviour
{
    public float arriveRadius = 0f;
    public override Vector3 CalculateSteeringForce()
    {
        /*calculate the direction*/
        /*Seek use target position - current position*/
        Vector3 direction = target.position - transform.position;

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
