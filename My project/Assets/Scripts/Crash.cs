using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Crash : MonoBehaviour
{
   AudioManager audioManager;

   private void Awake()
    {
        audioManager = GameObject.FindGameObjectWithTag("Audio").GetComponent<AudioManager>();
    } 
   private void OnCollisionEnter2D(Collision2D collision)
   {
        audioManager.PlaySFX(audioManager.death);
        Destroy(gameObject);
   }
}
