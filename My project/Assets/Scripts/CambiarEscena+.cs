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
    public void IrAInvitar()
    {
        SceneManager.LoadSceneAsync("Invitación");
    }
    public void IrAGameplay()
    {
        SceneManager.LoadSceneAsync("Gameplay");
    }
}