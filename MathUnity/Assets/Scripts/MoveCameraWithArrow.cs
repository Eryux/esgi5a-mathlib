using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveCameraWithArrow : MonoBehaviour {

    [SerializeField]
    [Range(0f,100f)]
    float moveSpeed = 2f;

    [SerializeField]
    [Range(0f, 100f)]
    float zoomSpeed = 2f;

	void FixedUpdate()
    {
        if (Input.GetKey(KeyCode.RightArrow))
        {
            transform.position = new Vector3(transform.position.x + moveSpeed * Time.deltaTime, transform.position.y, transform.position.z);
        }
        else if (Input.GetKey(KeyCode.LeftArrow))
        {
            transform.position = new Vector3(transform.position.x - moveSpeed * Time.deltaTime, transform.position.y, transform.position.z);
        }

        if (Input.GetKey(KeyCode.DownArrow))
        {
            transform.position = new Vector3(transform.position.x, transform.position.y, transform.position.z - moveSpeed * Time.deltaTime);
        }
        else if (Input.GetKey(KeyCode.UpArrow))
        {
            transform.position = new Vector3(transform.position.x, transform.position.y, transform.position.z + moveSpeed * Time.deltaTime);
        }

        if (Input.GetAxis("Mouse ScrollWheel") > 0f)
        {
            if (Camera.main.orthographicSize > 1.0f)
                Camera.main.orthographicSize -= zoomSpeed * Time.deltaTime;
        }
        else if (Input.GetAxis("Mouse ScrollWheel") < 0f)
        {
            Camera.main.orthographicSize += zoomSpeed * Time.deltaTime;
        }
    }
}
