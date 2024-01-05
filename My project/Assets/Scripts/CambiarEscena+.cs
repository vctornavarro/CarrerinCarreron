using UnityEngine;
using UnityEngine.SceneManagement;

public class CambiarEscena : MonoBehaviour
{
    public void IrAIniciarSesion()
    {
        SceneManager.LoadSceneAsync("IniciarSesion");
    }
    public void IrARegistrarse()
    {
        SceneManager.LoadSceneAsync("Registrarse");
    }
    public void IrAMenu()
    {
        SceneManager.LoadSceneAsync("MenuPrincipal");
    }
    public void IrAConsulta()
    {
        SceneManager.LoadSceneAsync("Consulta");
    }
    public void IrAMenuJuego()
    {
        SceneManager.LoadSceneAsync("MenuJuego");
    }
    public void IrAEleccion()
    {
        SceneManager.LoadSceneAsync("Eleccion");
    }
    public void IrAGameplay()
    {
        SceneManager.LoadSceneAsync("Gameplay");
    }
     public void IrAGameplay2()
    {
        SceneManager.LoadSceneAsync("Gameplay2");
    }
}