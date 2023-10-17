using Client;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using TMPro;
using UnityEngine;
using UnityEngine.SceneManagement;

public class CCliente : MonoBehaviour
{
    //mensajes
    public TextMeshProUGUI text1;

    //variables para registrarse/loguearse
    public TMP_InputField NameInput;
    public TMP_InputField PasswordInput;
    public TMP_InputField ConfirmPasswordInput;

    //variables para las consultas
    public TMP_InputField NombreJugadorInputField;
    public TMP_InputField IDJugadorInputField;
    public TMP_InputField NombreCircuitoInputField;
    public TextMeshProUGUI notificacion;
     
    private ConexionServidor conexionServidor;

    private void Awake()
    {
        conexionServidor = ConexionServidor.GetInstance();
        // Verificar si la instancia de conexionServidor es nula y crearla si es necesario
        if (conexionServidor == null)
        {
            conexionServidor = new ConexionServidor();
        }
    }
    private async void Start()
    {
        // Conectarse al servidor solo una vez
        if (!conexionServidor.IsConnected())
        {
            int connectionResult = conexionServidor.ConnectToServer();
            if (connectionResult == 0)
            {
                // La conexión se realizó correctamente
                Debug.Log("Connected to server");
            }
            else
            {
                // Error al conectar con el servidor
                Debug.Log("Failed to connect to server");
                return;// Sale del método Start si la conexión falla
            }
        }
        //continua con el resto del codigo
        await AtenderServidor();
    }

    private async Task AtenderServidor()
    {
        while (true)
        {
            string respuestaServidor = await Task.Run(() => conexionServidor.EscucharMensaje());
            Debug.Log(respuestaServidor);

            //codigo para separar el mensaje recibido del servidor
            string[] trozos = respuestaServidor.Split('-');
            int codigo = Convert.ToInt32(trozos[0]);
            string mensaje = trozos[1];

            switch (codigo)
            {
                case 0: //login
                    if (mensaje == "SI")
                    {

                        conexionServidor.SetLoggedIn(true);
                        Debug.Log("Se ha accedido correctamente a la cuenta");
                        SceneManager.LoadScene("MenuJuego");

                    }
                    else if (mensaje == "Error")
                    {
                        text1 = GameObject.Find("ErrorTxt").GetComponent<TextMeshProUGUI>();
                        text1.text = "";
                        Debug.Log(trozos[2]);
                        text1.text = "Error: " + trozos[2];
                    }
                    break;
                case 1: //registrar
                    if (mensaje == "SI")
                    {
                        conexionServidor.SetLoggedIn(true);
                        Debug.Log("Registrado Correctamente");
                        SceneManager.LoadScene("MenuJuego");
                    }
                    else if (mensaje == "Error")
                    {
                        text1 = GameObject.Find("Text").GetComponent<TextMeshProUGUI>();
                        text1.text = "";
                        text1.text = "Error: " + trozos[2];
                        Debug.Log("Problema al crear al usuario");
                    }
                    break;

            }
        }
    }

    public void IniciarSesion() //procedimiento para iniciar sesión
    {
        string Name = NameInput.text;
        string Password = PasswordInput.text;

        if ((Password != "") || Name != "")
        {
            string mensajeIniciarSesion = "0-" + Name + "-" + Password;
            conexionServidor.EnviarMensajeServidor(mensajeIniciarSesion);
            Debug.Log("Enviado");
        }
        else
        {
            text1 = GameObject.Find("Text").GetComponent<TextMeshProUGUI>();
            text1.text = "No has rellenado todos los apartados";
        }
    }

    public void Registrar() //procedimiento para registrarse
    {
        string Name = NameInput.text;
        string Password = PasswordInput.text;
        string PasswordConfirm = ConfirmPasswordInput.text;

        if ((Password != "") || (PasswordConfirm != "") || Name != "")
        {
            if ((Password == PasswordConfirm) && (PasswordInput != null) && (PasswordConfirm != null) && (Name != null))
            {
                string registrar = "1-" + Name + "-" + Password;
                conexionServidor.EnviarMensajeServidor(registrar);
                Debug.Log("Enviado");
            }
            else if (PasswordInput.text != ConfirmPasswordInput.text)
            {
                text1.text = "Las contraseñas no coinciden";
                Debug.Log("Las contraseñas no coinciden");
            }
        }
        else
        {
            text1 = GameObject.Find("Text").GetComponent<TextMeshProUGUI>();
            text1.text = "No has rellenado todos los apartados";
        }
    }

    public void Cerrar() //cerrar el juego desde el menú
    {
        conexionServidor.Desconectarse();
        Application.Quit();
    }
    public void DameTiempoJugado()
    {
        string IdeJugadorInputField = IDJugadorInputField.text;

        string query1 = "2-" + IDJugadorInputField;
        conexionServidor.EnviarMensajeServidor(query1);
        Debug.Log("Enviado");
    }
    public void DameCircuitoFav()
    {
        string NameJugadorInputField = NombreJugadorInputField.text;

        string query2 = "3-" + NameJugadorInputField;
        conexionServidor.EnviarMensajeServidor(query2);
        Debug.Log("Enviado");
    }
    public void DameDistanciaCircuito()
    {
        string NameCircuitInputField = NombreCircuitoInputField.text;

        string query3 = "4-" + NameCircuitInputField;
        conexionServidor.EnviarMensajeServidor(query3);
        Debug.Log("Enviado");
    }

}