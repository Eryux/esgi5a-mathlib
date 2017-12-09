using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PointObject : MonoBehaviour {

    [SerializeField]
    Material offMaterial;

    [SerializeField]
    Material selectMaterial;

    [SerializeField]
    Material markMaterial;

    [SerializeField]
    MeshRenderer render;

    bool marked = false;

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
        if (marked)
            Mark();
        else
            render.material = offMaterial;
    }

    public void Mark()
    {
        render.material = markMaterial;
        marked = true;
    }

    public void UnMark()
    {
        render.material = offMaterial;
        marked = false;
    }

}
