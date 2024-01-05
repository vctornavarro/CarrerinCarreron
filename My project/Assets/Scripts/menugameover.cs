using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using System;

public class menugameover : MonoBehaviour
{
    void Start()
    {

    }

    void Update()
    {
        
    }
    public GameObject gameOverUI;
  
   public void Reiniciar()
   {
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex);
   }

    public void MenuInicial(string nombre)
    {
        SceneManager.LoadScene(nombre);
    }

    public void gameOver()
    {
        gameOverUI.SetActive(true);
    }
    


}
