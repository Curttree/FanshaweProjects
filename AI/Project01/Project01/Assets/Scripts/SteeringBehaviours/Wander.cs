using UnityEngine;

public class Wander : SteeringBehaviour
{
    public float distanceToCircle;
    public float circleRadius;

    public override Vector3 CalculateSteeringForce()
    {
        float vehicleOrientation = rb.rotation.eulerAngles.y * Mathf.Deg2Rad;

        //calculate the circle's center point
        Vector3 circlePoint = transform.position + new Vector3(Mathf.Cos(-vehicleOrientation), 0, Mathf.Sin(-vehicleOrientation)) * distanceToCircle;

        //calculate a random spot on the circle's circumference
        float angle = Random.Range(0, Mathf.PI * 2);
        float x = Mathf.Sin(angle) * circleRadius;
        float z = Mathf.Cos(angle) * circleRadius;

        //the target point the wandering vehicle will seek towards
        Vector3 targetPosition = new Vector3(circlePoint.x + x, 0.5f, circlePoint.z + z);

        Vector3 direction = targetPosition - transform.position;

        Vector3 desiredVelocity = direction.normalized * maxSpeed;

        Vector3 steer = Vector3.ClampMagnitude(desiredVelocity - rb.velocity, maxForce);

        steer = steer / mass;

        return steer;
    }
}
