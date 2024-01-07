using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Movement : MonoBehaviour
{
    private Rigidbody2D rb;



    public float maximoX;
    public float minimoX;
    public float maximoY;
    public float minimoY;
    public float velocidadX;
    public float velocidadY;

    float movimientoX;
    float movimientoY;
    // Start is called before the first frame update
    void Start()
    {
        rb=GetComponent<Rigidbody2D>();
    }

    // Update is called once per frame
    void Update()
    {
        movimientoX = Input.GetAxis("Horizontal");
        movimientoY = Input.GetAxis("Vertical");

        transform.Translate(movimientoX * Time.deltaTime * velocidadX, movimientoY * Time.deltaTime * velocidadY, 0);
        transform.position = new Vector3(Mathf.Clamp(transform.position.x,minimoX,maximoX), Mathf.Clamp(transform.position.y,minimoY,maximoY),transform.position.z);
    }
    
        

    
    
}
