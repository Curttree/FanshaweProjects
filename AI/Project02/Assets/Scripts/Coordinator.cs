using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Coordinator : MonoBehaviour
{
    public Vehicle vehicle;
    public Vehicle[] followers; 
    
    private Vector3[] circleFormation; //position offset for circle formation
    private Vector3[] vFormation; //position offsets for v formation
    private Vector3[] squareFormation; //position offsets for square formation
    private Vector3[] lineFormation; //position offsets for v formation
    private Vector3[] rowsFormation; //position offsets for two rows formation
    private Vector3[] positionOffset; //holds the current formation

    private int lastFormation = 0;

    public bool isFollowingPath;
    public bool isFlocking;


    public Transform[] pathNodes;
    public int currentNode;
    private float pathRadius = 10.0f; 
    private int pathDirection = 1;

    public Material[] targetMaterials;
    private Quaternion lastRotation;

    // Start is called before the first frame update
    void Start()
    {
        // Warning message if something has gone wrong with the setup in the inspector.
        if (followers.Length != 12)
        {
            print("12 followers have not been set. Unexpected formations may occur.");
        }
        SetFlocking(isFlocking);
        //Populate formations
        positionOffset = new Vector3[12];

        float circleRadius = 30f;

        circleFormation = new Vector3[] { new Vector3(circleRadius * Mathf.Cos(0), 0, circleRadius * Mathf.Sin(0)),
                                        new Vector3(circleRadius * Mathf.Cos(Mathf.PI/6), 0, circleRadius * Mathf.Sin(Mathf.PI/6)),
                                        new Vector3(circleRadius * Mathf.Cos(Mathf.PI/3), 0, circleRadius * Mathf.Sin(Mathf.PI/3)),
                                        new Vector3(circleRadius * Mathf.Cos(Mathf.PI/2), 0, circleRadius * Mathf.Sin(Mathf.PI/2)),
                                        new Vector3(circleRadius * Mathf.Cos(2f * Mathf.PI/3f), 0, circleRadius * Mathf.Sin(2f * Mathf.PI/3f)),
                                        new Vector3(circleRadius * Mathf.Cos(5f * Mathf.PI/6f), 0, circleRadius * Mathf.Sin(5f * Mathf.PI/6f)),
                                        new Vector3(circleRadius * Mathf.Cos(Mathf.PI), 0, circleRadius * Mathf.Sin(Mathf.PI)),
                                        new Vector3(circleRadius * Mathf.Cos(7f * Mathf.PI/6f), 0, circleRadius * Mathf.Sin(7f * Mathf.PI/6f)),
                                        new Vector3(circleRadius * Mathf.Cos(4f * Mathf.PI/3f), 0, circleRadius * Mathf.Sin(4f * Mathf.PI/3f)),
                                        new Vector3(circleRadius * Mathf.Cos(3f * Mathf.PI/2f), 0, circleRadius * Mathf.Sin(3f * Mathf.PI/2f)),
                                        new Vector3(circleRadius * Mathf.Cos(5f * Mathf.PI/3f), 0, circleRadius * Mathf.Sin(5f * Mathf.PI/3f)),
                                        new Vector3(circleRadius * Mathf.Cos(11f * Mathf.PI/6f), 0, circleRadius * Mathf.Sin(11f * Mathf.PI/6f)) };

        squareFormation = new Vector3[] { new Vector3(-30, 0, -30),
                                        new Vector3(-10, 0, -30),
                                        new Vector3(10, 0, -30),
                                        new Vector3(30, 0, -30),
                                        new Vector3(30, 0, -10),
                                        new Vector3(30, 0, 10),
                                        new Vector3(30, 0, 30),
                                        new Vector3(10, 0, 30),
                                        new Vector3(-10, 0, 30),
                                        new Vector3(-30, 0, 30),
                                        new Vector3(-30, 0, 10),
                                        new Vector3(-30, 0, -10) };

        vFormation = new Vector3[] { new Vector3(10, 0, 0),
                                        new Vector3(-10, 0, 0),
                                        new Vector3(20, 0, -20),
                                        new Vector3(-20, 0, -20),
                                        new Vector3(30, 0, -40),
                                        new Vector3(-30, 0, -40),
                                        new Vector3(40, 0, -60),
                                        new Vector3(-40, 0, -60),
                                        new Vector3(50, 0, -80),
                                        new Vector3(-50, 0, -80),
                                        new Vector3(60, 0, -100),
                                        new Vector3(-60, 0, -100) };

        lineFormation = new Vector3[] { new Vector3(0, 0, 0),
                                        new Vector3(0, 0, 20),
                                        new Vector3(0, 0, 40),
                                        new Vector3(0, 0, 60),
                                        new Vector3(0, 0, 80),
                                        new Vector3(0, 0, 100),
                                        new Vector3(0, 0, -20),
                                        new Vector3(0, 0, -40),
                                        new Vector3(0, 0, -60),
                                        new Vector3(0, 0, -80),
                                        new Vector3(0, 0, -100),
                                        new Vector3(0, 0, -120) };

        rowsFormation = new Vector3[] { new Vector3(-40, 0, 0), 
                                        new Vector3(-20, 0, 0),
                                        new Vector3(0, 0, 0),
                                        new Vector3(20, 0, 0),
                                        new Vector3(40, 0, 0),
                                        new Vector3(60, 0, 0),
                                        new Vector3(-40, 0, 60),
                                        new Vector3(-20, 0, 60),
                                        new Vector3(0, 0, 60),
                                        new Vector3(20, 0, 60),
                                        new Vector3(40, 0, 60),
                                        new Vector3(60, 0, 60) };

        lastRotation = transform.rotation;
    }

    // Update is called once per frame
    void Update()
    {

        if (Input.GetKeyDown(KeyCode.Alpha1) || Input.GetKeyDown(KeyCode.Keypad1))
        {
            isFlocking = false;
            SetFlocking(isFlocking);
            GetInFormation(1);
        }
        else if (Input.GetKeyDown(KeyCode.Alpha2) || Input.GetKeyDown(KeyCode.Keypad2))
        {
            isFlocking = false;
            SetFlocking(isFlocking);
            GetInFormation(2);
        }
        else if (Input.GetKeyDown(KeyCode.Alpha3) || Input.GetKeyDown(KeyCode.Keypad3))
        {
            isFlocking = false;
            SetFlocking(isFlocking);
            GetInFormation(3);
        }
        else if (Input.GetKeyDown(KeyCode.Alpha4) || Input.GetKeyDown(KeyCode.Keypad4))
        {
            isFlocking = false;
            SetFlocking(isFlocking);
            GetInFormation(4);
        }
        else if (Input.GetKeyDown(KeyCode.Alpha5) || Input.GetKeyDown(KeyCode.Keypad5))
        {
            isFlocking = false;
            SetFlocking(isFlocking);
            GetInFormation(5);
        }
        else if (Input.GetKeyDown(KeyCode.Alpha6) || Input.GetKeyDown(KeyCode.Keypad6))
        {
            isFlocking = true;
            SetFlocking(isFlocking);
        }
        else if (Input.GetKeyDown(KeyCode.Alpha7) || Input.GetKeyDown(KeyCode.Keypad7))
        {
            isFlocking = false;
            SetFlocking(isFlocking);
        }
        else if (Input.GetKeyDown(KeyCode.Alpha8) || Input.GetKeyDown(KeyCode.Keypad8))
        {
            isFlocking = false;
            SetFlocking(isFlocking);
            pathNodes[currentNode].gameObject.GetComponent<MeshRenderer>().material = targetMaterials[1];
            isFollowingPath = true; 
            for (int x = 0; x < positionOffset.Length; x++)
            {
                Quaternion newRotation = transform.rotation * Quaternion.Inverse(Quaternion.Euler(0, 0, 0));

                // Rotate all of our formations first.
                circleFormation[x] = newRotation * circleFormation[x];
                vFormation[x] = newRotation * vFormation[x];
                squareFormation[x] = newRotation * squareFormation[x];
                lineFormation[x] = newRotation * lineFormation[x];
                rowsFormation[x] = newRotation * rowsFormation[x];

                positionOffset[x] = GetOriginalPosition(x);
            }
            lastRotation = transform.rotation;

        }
        else if (Input.GetKeyDown(KeyCode.Alpha9) || Input.GetKeyDown(KeyCode.Keypad9))
        {
            pathDirection *= -1;
        }
        else if (Input.GetKeyDown(KeyCode.Alpha0) || Input.GetKeyDown(KeyCode.Keypad0))
        {
            pathNodes[currentNode].gameObject.GetComponent<MeshRenderer>().material = targetMaterials[0];
            isFollowingPath = false;
            for (int x = 0; x < positionOffset.Length; x++)
            {
                Quaternion newRotation = Quaternion.Euler(0, 0, 0) * Quaternion.Inverse(lastRotation);

                // Rotate all of our formations first.
                circleFormation[x] = newRotation * circleFormation[x];
                vFormation[x] = newRotation * vFormation[x];
                squareFormation[x] = newRotation * squareFormation[x];
                lineFormation[x] = newRotation * lineFormation[x];
                rowsFormation[x] = newRotation * rowsFormation[x];

                positionOffset[x] = GetOriginalPosition(x);
            }
            GetInFormation(lastFormation);
        }
        else if (Input.GetKeyDown(KeyCode.Plus) || Input.GetKeyDown(KeyCode.KeypadPlus))
        {
            isFlocking = true;
            SetCombination(isFlocking);
            gameObject.GetComponent<Seek>().enabled = true;
            pathNodes[currentNode].gameObject.GetComponent<MeshRenderer>().material = targetMaterials[1];
            isFollowingPath = true;
        }
        else if (Input.GetKeyDown(KeyCode.Minus) || Input.GetKeyDown(KeyCode.KeypadMinus))
        {
            isFollowingPath = false;
            pathNodes[currentNode].gameObject.GetComponent<MeshRenderer>().material = targetMaterials[0];
            isFlocking = false;
            SetCombination(isFlocking);
        }


        if (isFollowingPath && lastFormation > 0)
        {
            Vector3 target = FindTarget();
            vehicle.Seek(target);
            if (!isFlocking)
            {
                //TODO: Rotate position depending on direction of movement.
                for (int x = 0; x < positionOffset.Length; x++)
                {
                    Quaternion newRotation = transform.rotation * Quaternion.Inverse(lastRotation);

                    // Rotate all of our formations first.
                    circleFormation[x] = newRotation * circleFormation[x];
                    vFormation[x] = newRotation * vFormation[x];
                    squareFormation[x] = newRotation * squareFormation[x];
                    lineFormation[x] = newRotation * lineFormation[x];
                    rowsFormation[x] = newRotation * rowsFormation[x];

                    positionOffset[x] = GetOriginalPosition(x);
                }

                lastRotation = transform.rotation;
                for (int x = 0; x < positionOffset.Length; x++)
                {
                    if (x < followers.Length)
                    {
                        followers[x].Seek(transform.position + positionOffset[x]);
                    }
                }
            }
        }
        else
        {
            vehicle.Seek(transform.position);
        }
    }

    void GetInFormation(int form)
    {
        switch (form)
        {
            case 1:
                positionOffset = circleFormation;
                break;
            case 2:
                positionOffset = vFormation;
                break;
            case 3:
                positionOffset = squareFormation;
                break;
            case 4:
                positionOffset = lineFormation;
                break;
            case 5:
                positionOffset = rowsFormation;
                break;
            default:
                // invalid selection. Return without doing anything.
                return;
        }

        lastFormation = form;

        for (int x = 0; x < positionOffset.Length; x++)
        {
            if (x < followers.Length)
            {
                followers[x].Seek(transform.position + positionOffset[x]);
            }
        }
    }

    Vector3 GetOriginalPosition(int index)
    {
        // Gets the original formation position from the appropriate vector array.
        switch (lastFormation)
        {
            case 1:
                return circleFormation[index];
            case 2:
                return vFormation[index];
            case 3:
                return squareFormation[index];
            case 4:
                return lineFormation[index];
            case 5:
                return rowsFormation[index];
            default:
                // invalid selection. Return without doing anything.
                return Vector3.zero;
        }
    }

    Vector3 FindTarget()
    {
        Vector3 target = pathNodes[currentNode].position;
        if (Vector3.Distance(transform.position, target) < pathRadius)
        {
            pathNodes[currentNode].gameObject.GetComponent<MeshRenderer>().material = targetMaterials[0];
            currentNode += pathDirection;
            if (currentNode >= pathNodes.Length)
            {
                currentNode = pathNodes.Length - 1;
            }
            if (currentNode < 0)
            {
                currentNode = 0;
            }
            pathNodes[currentNode].gameObject.GetComponent<MeshRenderer>().material = targetMaterials[1];
        }
        return target;
    }

    private void SetFlocking(bool value)
    {
        if (value)
        {

            foreach (Seek childSeek in GetComponentsInChildren<Seek>())
            {
                childSeek.enabled = false;
            }
            foreach (Flocking childFlock in GetComponentsInChildren<Flocking>())
            {
                childFlock.enabled = true;
            }
        }
        else
        {

            foreach (Seek childSeek in GetComponentsInChildren<Seek>())
            {
                childSeek.enabled = true;
            }
            foreach (Flocking childFlock in GetComponentsInChildren<Flocking>())
            {
                childFlock.enabled = false;
            }
        }
    }

    private void SetCombination(bool value)
    {
        if (value)
        {

            foreach (Seek childSeek in GetComponentsInChildren<Seek>())
            {
                childSeek.enabled = false;
            }
            foreach (Flocking childFlock in GetComponentsInChildren<Flocking>())
            {
                childFlock.enabled = true;
            }
        }
        else
        {

            foreach (Seek childSeek in GetComponentsInChildren<Seek>())
            {
                childSeek.enabled = true;
            }
            foreach (Flocking childFlock in GetComponentsInChildren<Flocking>())
            {
                childFlock.enabled = false;
            }
        }

    }
}
