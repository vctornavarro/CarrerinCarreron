using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Parallax : MonoBehaviour
{
    public Vector2 parallaxVelocity;
    public MeshRenderer meshRenderer;

    //Update is called once per frame

    void Update()
    {
        meshRenderer.material.mainTextureOffset = parallaxVelocity * Time.time;
    }
}
