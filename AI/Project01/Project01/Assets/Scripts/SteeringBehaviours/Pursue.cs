using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Pursue : SteeringBehaviour
{
    public float T_maximumsteps;
    public override Vector3 CalculateSteeringForce()
    {
        Vector3 direction = target.position - transform.position;
        float distance = direction.magnitude;

        float speed = rb.velocity.magnitude;

        float T;

        if(speed <= distance / T_maximumsteps)
        {
            T = T_maximumsteps;  //target is far away
        }
        else
        {
            T = distance / speed; //target is close
        }

        //float T = T_maximumsteps;
        //calculate the future position the vehicle will pursue towards
        Vector3 futurePosition = target.position + target.gameObject.GetComponent<Rigidbody>().velocity * T;

        /*now we seek towards this future position*/
        Vector3 directionToFuturePosition = futurePosition - transform.position;

        Vector3 desiredVelocity = directionToFuturePosition.normalized * maxSpeed;

        Vector3 steer = Vector3.ClampMagnitude(desiredVelocity - rb.velocity, maxForce);

        steer = steer / mass;

        return steer;
    }
}
