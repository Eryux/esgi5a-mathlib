using System;
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

    [DllImport("MathLib_Unity", EntryPoint = "jarvis_walk")]
    static extern IntPtr jarvisWalk(IntPtr points, int size, IntPtr return_size);

    // Use this for initialization
    void Start ()
    {
        points = new List<PointObject>();
	}
	
	// Update is called once per frame
	void Update ()
    {
		if (Input.GetKeyDown(KeyCode.P))
            AddPoint();

        if (Input.GetKeyDown(KeyCode.X))
            RemovePoint();

        if (Input.GetKeyDown(KeyCode.J))
            Action_JarvisWalk();

        if (Input.GetKeyDown(KeyCode.Delete))
            ClearAllPoint();

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

    // ------------------------------------

    public void Action_JarvisWalk()
    {
        float[] p = new float[points.Count * 2];
        for (int i = 0; i < points.Count; i++) {
            p[i * 2] = points[i].transform.position.x;
            p[i * 2 + 1] = points[i].transform.position.z;
        }

        IntPtr raw_points = Marshal.AllocHGlobal(sizeof(float) * p.Length);
        Marshal.Copy(p, 0, raw_points, p.Length);

        IntPtr return_size = Marshal.AllocHGlobal(sizeof(int));
        IntPtr result = jarvisWalk(raw_points, p.Length / 2, return_size);
        Marshal.FreeHGlobal(raw_points);

        if (result != IntPtr.Zero)
        {
            int result_size = Marshal.ReadInt32(return_size);

            int[] final_result = new int[result_size];
            Marshal.Copy(result, final_result, 0, result_size);

            Debug.Log("Jarvis - Points : " + result_size);

            for (int i = 0; i < points.Count; i++)
            {
                points[i].UnMark();
            }

            for (int i = 0; i < result_size; i += 2)
            {
                points[final_result[i] / 2].Mark();
            }

            Marshal.FreeHGlobal(return_size);
            Marshal.FreeHGlobal(result);
        }
        else
        {
            Debug.LogWarning("3 points are required for performing Jarvis walk.");
        }
    }

    // ------------------------------------

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

    public void ClearAllPoint()
    {
        foreach (var pobject in points) {
            Destroy(pobject.gameObject);
        }

        points.Clear();
    }

    // ------------------------------------

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
