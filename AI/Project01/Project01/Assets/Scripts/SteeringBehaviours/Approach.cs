using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Approach : SteeringBehaviour
{
    public float stoppingRadius;
    public bool insideStoppingRadius;
    public override Vector3 CalculateSteeringForce()
    {
        Vector3 desiredVelocity = target.position - transform.position;
        //desiredVelocity = desiredVelocity.normalized * maxSpeed;

        float distance = desiredVelocity.magnitude;

        //Check the distance to see if we are inside the slowing radius
        if(distance < stoppingRadius) {
            // If we wished to slow rather than stop, we would do the following:

            //inside the slowing area
            //desiredVelocity *= (distance / slowingRadius);

            // In our case we want to stop and maintain the given radius. Set velocity to 0 instead.
            insideStoppingRadius = true;
            return -rb.velocity;
        }
        else
        {
            insideStoppingRadius = false;
        }

        desiredVelocity = desiredVelocity.normalized * maxSpeed;

        Vector3 steer = Vector3.ClampMagnitude(desiredVelocity - rb.velocity, maxForce);

        steer = steer / mass;
        
        return steer;
    }
}
