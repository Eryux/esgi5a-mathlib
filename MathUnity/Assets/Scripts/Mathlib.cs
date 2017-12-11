using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.IO;
using UnityEngine;
using UnityEngine.UI;

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

    [SerializeField]
    MeshCreator meshCreate;

    [SerializeField]
    Dropdown convexeMethodSelect;

    [SerializeField]
    Dropdown triangulationMedthodSelect;

    [SerializeField]
    Toggle drawVoronoi;
    
    public List<PointObject> points;

    bool activeAddPoint = false;

    PointObject selectedPoint = null;

    [DllImport("MathLib_Unity", EntryPoint = "classic_pow")]
    static extern int classic_pow(int a, int p);

    [DllImport("MathLib_Unity", EntryPoint = "jarvis_walk")]
    static extern IntPtr jarvisWalk(IntPtr points, int size, IntPtr return_size);

    [DllImport("MathLib_Unity", EntryPoint = "graham_scan")]
    static extern IntPtr grahamScan(IntPtr points, int size, IntPtr return_size);

    [DllImport("MathLib_Unity", EntryPoint = "triangulate")]
    static extern IntPtr triangulate(IntPtr points, int size, IntPtr return_size);

    [DllImport("MathLib_Unity", EntryPoint = "triangulate_delaunay")]
    static extern IntPtr triangulateDelaunay(IntPtr points, int size, IntPtr return_size);

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
            ClearAllPoint();

        if (Input.GetKeyDown(KeyCode.Delete))
            RemovePoint();

        if (Input.GetKeyDown(KeyCode.J))
            Action_JarvisWalk();

        if (Input.GetKeyDown(KeyCode.G))
            Action_GrahamScan();

        if (Input.GetKeyDown(KeyCode.T))
            Action_Triangulate();

        if (Input.GetKeyDown(KeyCode.D))
            Action_TriangulateDelaunay();

        if (Input.GetMouseButtonDown(2))
        {
            if (selectedPoint != null)
            {
                Vector3 pos = Camera.main.ScreenToWorldPoint(new Vector3(Input.mousePosition.x, Input.mousePosition.y, Camera.main.nearClipPlane));
                pos.y = 0f;
                selectedPoint.transform.position = pos;
                Refresh();
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

    public void Refresh()
    {
        if (points.Count < 3) return;

        if (convexeMethodSelect.value == 1)
        {
            Action_JarvisWalk();
        }
        else if (convexeMethodSelect.value == 2)
        {
            Action_GrahamScan();
        }

        if (triangulationMedthodSelect.value == 1)
        {
            Action_Triangulate();
        }
        else if (triangulationMedthodSelect.value == 2)
        {
            Action_TriangulateDelaunay();
        }

        if (drawVoronoi.isOn)
        {
            // Draw voronoi
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

        SaveTestInputInFile(p);

        IntPtr raw_points = Marshal.AllocHGlobal(sizeof(float) * p.Length);
        Marshal.Copy(p, 0, raw_points, p.Length);

        IntPtr return_size = Marshal.AllocHGlobal(sizeof(int));
        float timer = Time.time * 1000;
        IntPtr result = jarvisWalk(raw_points, p.Length / 2, return_size);
        float elapsed_time = Time.time * 1000 - timer;
        Marshal.FreeHGlobal(raw_points);

        if (result != IntPtr.Zero)
        {
            int result_size = Marshal.ReadInt32(return_size);

            int[] final_result = new int[result_size];
            Marshal.Copy(result, final_result, 0, result_size);

            Debug.Log("Jarvis - Elapsed time : " + elapsed_time + " ms - Points : " + (result_size / 2));

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

    public void Action_GrahamScan()
    {
        float[] p = new float[points.Count * 2];
        for (int i = 0; i < points.Count; i++)
        {
            p[i * 2] = points[i].transform.position.x;
            p[i * 2 + 1] = points[i].transform.position.z;
        }

        SaveTestInputInFile(p);

        IntPtr raw_points = Marshal.AllocHGlobal(sizeof(float) * p.Length);
        Marshal.Copy(p, 0, raw_points, p.Length);

        IntPtr return_size = Marshal.AllocHGlobal(sizeof(int));
        IntPtr result = grahamScan(raw_points, p.Length / 2, return_size);
        Marshal.FreeHGlobal(raw_points);

        if (result != IntPtr.Zero)
        {
            int result_size = Marshal.ReadInt32(return_size);

            int[] final_result = new int[result_size];
            Marshal.Copy(result, final_result, 0, result_size);

            Debug.Log("Graham Scan - Points : " + result_size);

            for (int i = 0; i < points.Count; i++)
            {
                points[i].UnMark();
            }

            for (int i = 0; i < result_size; i++)
            {
                points[final_result[i]].Mark();
            }

            Marshal.FreeHGlobal(return_size);
            Marshal.FreeHGlobal(result);
        }
        else
        {
            Debug.LogWarning("3 points are required for performing Graham scan.");
        }
    }

    public void Action_Triangulate()
    {
        float[] p = new float[points.Count * 2];
        for (int i = 0; i < points.Count; i++)
        {
            p[i * 2] = points[i].transform.position.x;
            p[i * 2 + 1] = points[i].transform.position.z;
        }

        SaveTestInputInFile(p);

        IntPtr raw_points = Marshal.AllocHGlobal(sizeof(float) * p.Length);
        Marshal.Copy(p, 0, raw_points, p.Length);

        IntPtr return_size = Marshal.AllocHGlobal(sizeof(int));
        IntPtr result = triangulate(raw_points, p.Length / 2, return_size);
        Marshal.FreeHGlobal(raw_points);

        if (result != IntPtr.Zero)
        {
            int result_size = Marshal.ReadInt32(return_size);

            float[] final_result = new float[result_size];
            Marshal.Copy(result, final_result, 0, result_size);

            Debug.Log("Triangulate - Points : " + result_size / 2 + " - Triangle : " + result_size / 6);

            for (int i = 0; i < result_size; i += 6)
            {
                Debug.Log("Triangle " + i / 6 + " : " + final_result[i] + "," + final_result[i + 1] + " - " + final_result[i + 2] + "," + final_result[i + 3] + " - " + final_result[i + 4] + "," + final_result[i + 5]);
            }

            meshCreate.Create2D(final_result);

            Marshal.FreeHGlobal(return_size);
            Marshal.FreeHGlobal(result);
        }
        else
        {
            Debug.LogWarning("3 points are required for performing triangulation.");
        }
    }

    public void Action_TriangulateDelaunay()
    {
        float[] p = new float[points.Count * 2];
        for (int i = 0; i < points.Count; i++)
        {
            p[i * 2] = points[i].transform.position.x;
            p[i * 2 + 1] = points[i].transform.position.z;
        }

        SaveTestInputInFile(p);

        IntPtr raw_points = Marshal.AllocHGlobal(sizeof(float) * p.Length);
        Marshal.Copy(p, 0, raw_points, p.Length);

        IntPtr return_size = Marshal.AllocHGlobal(sizeof(int));
        IntPtr result = triangulateDelaunay(raw_points, p.Length / 2, return_size);
        Marshal.FreeHGlobal(raw_points);

        if (result != IntPtr.Zero)
        {
            int result_size = Marshal.ReadInt32(return_size);

            float[] final_result = new float[result_size];
            Marshal.Copy(result, final_result, 0, result_size);

            Debug.Log("Triangulate - Points : " + result_size / 2 + " - Triangle : " + result_size / 6);

            for (int i = 0; i < result_size; i += 6)
            {
                Debug.Log("Triangle " + i / 6 + " : " + final_result[i] + "," + final_result[i + 1] + " - " + final_result[i + 2] + "," + final_result[i + 3] + " - " + final_result[i + 4] + "," + final_result[i + 5]);
            }

            meshCreate.Create2D(final_result);

            Marshal.FreeHGlobal(return_size);
            Marshal.FreeHGlobal(result);
        }
        else
        {
            Debug.LogWarning("3 points are required for performing triangulation.");
        }
    }

    // ------------------------------------

    void SaveTestInputInFile(float[] data)
    {
        /*if (File.Exists(Application.dataPath + "/test.txt"))
        {
            File.Create(Application.dataPath + "/text.txt");
        }

        using (StreamWriter file = new StreamWriter(Application.dataPath + "/test.txt", true))
        {
            string line = "";
            for (int i = 0; i < data.Length; i++) { line += data[i].ToString("0.00") + "f, "; }
            file.WriteLine(line);
            file.Close();
        }*/
    }

    // ------------------------------------

    public void RemovePoint()
    {
        if (selectedPoint == null) { return; }

        var p = selectedPoint;
        points.Remove(p);
        Destroy(p.gameObject);

        Refresh();
    }

    public void ToggleAddPoint()
    {
        activeAddPoint = !activeAddPoint;
    }

    public void ClearAllPoint()
    {
        meshCreate.Reset();

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

        Refresh();
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
