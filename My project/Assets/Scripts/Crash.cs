using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using UnityEngine.SceneManagement;

public class Crash : MonoBehaviour
{
   AudioManager audioManager;
   public menugameover gameManager;


   /*private void Awake()
    {
        audioManager = GameObject.FindGameObjectWithTag("Audio").GetComponent<AudioManager>();
    } 
    */
   private void OnCollisionEnter2D(Collision2D collision)
   {
       
        
        Destroy(gameObject);
        gameManager.gameOver();
        
       // audioManager.PlaySFX(audioManager.death);
   }
}
