using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class Mathlib : MonoBehaviour {

    #region UnityCompliant Singleton
    public static Mathlib Instance
    {
        get;
        private set;
    }

    public virtual void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
            return;
        }

        Destroy(this.gameObject);
    }
    #endregion

    [SerializeField]
    GameObject pointObjectPrefab;

    [SerializeField]
    Transform pointsWrapper;
    
    public List<PointObject> points;

    bool activeAddPoint = false;

    PointObject selectedPoint = null;

    [DllImport("MathLib_Unity", EntryPoint = "classic_pow")]
    static extern int classic_pow(int a, int p);

    // Use this for initialization
    void Start ()
    {
        points = new List<PointObject>();
        Debug.Log("MathLib Call > classic_pow(10, 3) : " + classic_pow(10, 3));
	}
	
	// Update is called once per frame
	void Update ()
    {
		if (Input.GetKeyDown(KeyCode.P))
        {
            AddPoint();
        }

        if (Input.GetKeyDown(KeyCode.X))
        {
            RemovePoint();
        }

        if (Input.GetMouseButtonDown(2))
        {
            if (selectedPoint != null)
            {
                Vector3 pos = Camera.main.ScreenToWorldPoint(new Vector3(Input.mousePosition.x, Input.mousePosition.y, Camera.main.nearClipPlane));
                pos.y = 0f;
                selectedPoint.transform.position = pos;
            }
        }

        if (Input.GetMouseButtonDown(0))
        {
            if (activeAddPoint)
            {
                AddPoint();
                activeAddPoint = false;
            }
            else
            {
                var ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                RaycastHit hit;
                if (Physics.Raycast(ray, out hit))
                {
                    PointObject point = hit.collider.gameObject.GetComponent<PointObject>();
                    if (point != null)
                    {
                        SelectPoint(point);
                    }
                }
            }
        }

        if (Input.GetKeyDown(KeyCode.Escape))
        {
            if (selectedPoint)
            {
                selectedPoint.Release();
                selectedPoint = null;
            }

            if (activeAddPoint)
            {
                activeAddPoint = false;
            }
        }
	}

    public void RemovePoint()
    {
        if (selectedPoint == null) { return; }

        var p = selectedPoint;
        points.Remove(p);
        Destroy(p.gameObject);
    }

    public void ToggleAddPoint()
    {
        activeAddPoint = !activeAddPoint;
    }

    void AddPoint()
    {
        Vector3 pos = Camera.main.ScreenToWorldPoint(new Vector3(Input.mousePosition.x, Input.mousePosition.y, Camera.main.nearClipPlane));
        pos.y = 0f;

        GameObject o = Instantiate(pointObjectPrefab, pos, Quaternion.identity) as GameObject;
        o.transform.parent = pointsWrapper;
        PointObject p = o.GetComponent<PointObject>();
        points.Add(p);
        SelectPoint(p);
    }

    void SelectPoint(PointObject p)
    {
        if (selectedPoint != null)
        {
            selectedPoint.Release();
        }

        selectedPoint = p;
        p.Select();
    }

}
