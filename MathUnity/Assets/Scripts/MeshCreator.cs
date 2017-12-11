using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MeshCreator : MonoBehaviour {

    [SerializeField]
    MeshFilter mf;

    public void Create2D(float[] points)
    {
        mf.mesh = new Mesh();

        Vector3[] vertices = new Vector3[points.Length / 2];
        for (int i = 0; i < points.Length / 2; i++)
        {
            vertices[i] = new Vector3(points[i * 2], 0f, points[i * 2 + 1]);
        }

        mf.mesh.vertices = vertices;

        int[] triangles = new int[vertices.Length];
        for (int i = 0; i < vertices.Length; i++)
        {
            triangles[i] = i;
        }

        mf.mesh.triangles = triangles;

        Vector3[] normals = new Vector3[vertices.Length];
        for (int i = 0; i < vertices.Length; i++)
        {
            normals[i] = Vector3.down;
        }

        mf.mesh.normals = normals;
    }

    public void Reset()
    {
        mf.mesh = null;
    }

}
