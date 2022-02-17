using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Flocking : SteeringBehaviour
{
    public float sepRadius = 1.0f;
    public float aliRadius = 1.0f;
    public float cohRadius = 1.0f;

    public float sepWeight = 1.0f;
    public float aliWeight = 1.0f;
    public float cohWeight = 1.0f;

    public float weightingAdjustment = 0.1f;

    private float sepCount = 1f;
    private float aliCount = 1f;
    private float cohCount = 1f;

    private void Start()
    {
        CalculateWeights();
    }

    private void Update()
    {
        if (Input.GetKeyDown(KeyCode.Z))
        {
            cohCount += weightingAdjustment;
        }
        if (Input.GetKeyDown(KeyCode.X))
        {
            cohCount-= weightingAdjustment;
            if (cohCount < 0)
            {
                // Don't let the user set a negative value
                cohCount = 0;
            }
        }
        if (Input.GetKeyDown(KeyCode.C))
        {
            sepCount+= weightingAdjustment;
        }
        if (Input.GetKeyDown(KeyCode.V))
        {
            sepCount-= weightingAdjustment;
            if (sepCount < 0)
            {
                // Don't let the user set a negative value
                sepCount = 0;
            }
        }
        if (Input.GetKeyDown(KeyCode.B))
        {
            aliCount += weightingAdjustment;
        }
        if (Input.GetKeyDown(KeyCode.N))
        {
            aliCount -= weightingAdjustment;
            if (aliCount < 0)
            {
                // Don't let the user set a negative value
                aliCount = 0;
            }
        }


        CalculateWeights();
    }

    private void CalculateWeights()
    {
        sepWeight = sepCount / SumFactors();
        aliWeight = aliCount / SumFactors();
        cohWeight = cohCount / SumFactors();
    }
    private float SumFactors()
    {
        return sepCount + aliCount + cohCount;
    }
    private Vector3 Separate()
    {
        Vector3 totalFlee = Vector3.zero;
        int neighbourhoodCount = 0;
        foreach(Transform i in transform.parent)
        {
            float distance = Vector3.Distance(rb.position, i.position);

            if(distance < sepRadius && distance > 0.0f) //the boid is inside our neighbourhood
            {
                Vector3 fleeVector = rb.position - i.position;
                fleeVector.Normalize();
                fleeVector /= distance;
                totalFlee += fleeVector;
                neighbourhoodCount++;
            }
        }

        Vector3 sepForce = Vector3.zero;
        if(neighbourhoodCount > 0)
        {
            Vector3 desiredVelocity = totalFlee / neighbourhoodCount;
            desiredVelocity.Normalize();
            desiredVelocity *= maxSpeed;
            sepForce = Vector3.ClampMagnitude(desiredVelocity - rb.velocity, maxForce);
            sepForce = sepForce / mass;
        }
        return sepForce;
    }


    private Vector3 Align()
    {
        Vector3 totalVelocity = Vector3.zero;
        int neighbourhoodCount = 0;
        foreach (Transform i in transform.parent)
        {
            float distance = Vector3.Distance(rb.position, i.position);

            if (distance < aliRadius && distance > 0.0f) //the boid is inside our neighbourhood
            {
                totalVelocity += i.GetComponent<Rigidbody>().velocity;
                neighbourhoodCount++;
            }
        }

        Vector3 aliForce = Vector3.zero;
        if (neighbourhoodCount > 0)
        {
            Vector3 desiredVelocity = totalVelocity / neighbourhoodCount;
            desiredVelocity.Normalize();
            desiredVelocity *= maxSpeed;
            aliForce = Vector3.ClampMagnitude(desiredVelocity - rb.velocity, maxForce);
            aliForce = aliForce / mass;
        }
        return aliForce;
    }

    private Vector3 Cohesion()
    {
        Vector3 totalPosition = Vector3.zero;
        int neighbourhoodCount = 0;
        foreach (Transform i in transform.parent)
        {
            float distance = Vector3.Distance(rb.position, i.position);

            if (distance < cohRadius && distance > 0.0f) //the boid is inside our neighbourhood
            {
                totalPosition += i.position;
                neighbourhoodCount++;
            }
        }

        Vector3 cohForce = Vector3.zero;
        if (neighbourhoodCount > 0)
        {
            Vector3 targetPos = totalPosition / neighbourhoodCount;

            Vector3 direction = targetPos - transform.position;

            Vector3 desiredVelocity = direction.normalized * maxSpeed;

            cohForce = Vector3.ClampMagnitude(desiredVelocity - rb.velocity, maxForce);
            cohForce = cohForce / mass;
        }
        return cohForce;
    }

    public override Vector3 CalculateSteeringForce()
    {
        Vector3 steer = Vector3.zero;

        //calculate flocking behaviours
        steer += Separate() * sepWeight;
        steer += Align() * aliWeight;
        steer += Cohesion() * cohWeight;

        return steer;
    }
}
