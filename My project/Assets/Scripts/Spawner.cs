using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Spawner : MonoBehaviour
{
    [SerializeField] GameObject carToGenerate;
    [SerializeField] float initialTimeToSpawn;
    [SerializeField] float repeatTimeToSpawn;
    // Start is called before the first frame update
    void Start()
    {
        InvokeRepeating("CarCreate", initialTimeToSpawn, repeatTimeToSpawn);
    }

    void CarCreate()
    {
        Instantiate(carToGenerate, transform.position, Quaternion.identity);
    }
   
}
