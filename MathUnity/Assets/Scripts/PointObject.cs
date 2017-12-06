using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PointObject : MonoBehaviour {

    [SerializeField]
    Material offMaterial;

    [SerializeField]
    Material selectMaterial;

    [SerializeField]
    MeshRenderer render;

    void Start () {
        
	}
	
	void Update () {
		
	}

    public void Select()
    {
        render.material = selectMaterial;
    }

    public void Release()
    {
        render.material = offMaterial;
    }

}
