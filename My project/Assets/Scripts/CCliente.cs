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
    public TextMeshProUGUI text2;
    public TextMeshProUGUI text3;
    public TextMeshProUGUI text4;
    public TextMeshProUGUI text5;

    //variables para registrarse/loguearse
    public TMP_InputField NameInput;
    public TMP_InputField PasswordInput;
    public TMP_InputField ConfirmPasswordInput;

    //variables para las consultas
    public TMP_InputField NombreJugadorInputField;
    public TMP_InputField NombreCircuitoInputField;
    public TextMeshProUGUI notificacion;
    public TMP_InputField NJInputField;

    private ConexionServidor conexionServidor;
    private string usuario;
    private string contrasena;

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
                // La conexion se realizo correctamente
                Debug.Log("Connected to server");
            }
            else
            {
                // Error al conectar con el servidor
                Debug.Log("Failed to connect to server");
                return;// Sale del metodo Start si la conexion falla
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
                case 2://consulta: Dame el tiempo total jugado por un jugador
                    if (mensaje == "SI")
                    {
                        string tiempototal = trozos[2];
                        Debug.Log("Tiempo total: " + tiempototal);
                        text1 = GameObject.Find("Text Consulta1").GetComponent<TextMeshProUGUI>();
                        text1.text = "";
                        text1.text = "El tiempo total jugado es de " + tiempototal;
                    }
                    else if (mensaje == "Error")
                    {
                        text1.text = "No se encuentran datos que coincidan";
                        text1 = GameObject.Find("Text Consulta1").GetComponent<TextMeshProUGUI>();
                        //text1.text = "";
                        Debug.Log("No se encuentran datos que coincidan");
                    }
                    break;
                case 3://consulta: Dame el circuito favorito de un jugador
                    if (mensaje == "SI")
                    {
                        string circuitofav = trozos[2];
                        Debug.Log("Circuito favorito: " + circuitofav);
                        text2 = GameObject.Find("Text Consulta2").GetComponent<TextMeshProUGUI>();
                        text2.text = "";
                        text2.text = "El circuito favorito es " + circuitofav;
                    }
                    else if (mensaje == "Error")
                    {
                        text2.text = "No se encuentran datos que coincidan";
                        text2 = GameObject.Find("Text Consulta2").GetComponent<TextMeshProUGUI>();
                        //text2.text = "";
                        Debug.Log("No se encuentran datos que coincidan");
                    }
                    break;
                case 4://consulta: Dame la duracion total de un circuito
                    if (mensaje == "SI")
                    {
                        string duracion = trozos[2];
                        Debug.Log("Duracion total del circuito: " + duracion);
                        text3 = GameObject.Find("Text Consulta3").GetComponent<TextMeshProUGUI>();
                        text3.text = "";
                        text3.text = "El tiempo total del circuito es de " + duracion;
                    }
                    else if (mensaje == "Error")
                    {
                        text3.text = "No se encuentran datos que coincidan";
                        text3 = GameObject.Find("Text Consulta3").GetComponent<TextMeshProUGUI>();
                        //text3.text = "";
                        Debug.Log("No se encuentran datos que coincidan");
                    }
                    break;
                case 5:
                    List<string> jugadoresConectados = new List<string>();
                    for (int i = 2; i < trozos.Length; i++)
                    {
                        jugadoresConectados.Add(trozos[i]);
                    }
                    string STRjugadoresConectados = "";

                    foreach (string jugador in jugadoresConectados)
                    {
                        STRjugadoresConectados += jugador + ", ";
                    }
                    text4 = GameObject.Find("Text Conectados").GetComponent<TextMeshProUGUI>();
                    text4.text = "";
                    text4.text = "Jugadores conectados: " + STRjugadoresConectados;

                    /*text5 = GameObject.Find("ListaConectados").GetComponent<TextMeshProUGUI>();
                    text5.text = "";
                    text5.text = STRjugadoresConectados;*/
                    break;
                case 6: //desconectar
                    if (mensaje == "Desconectando")
                    {
                        conexionServidor.SetLoggedIn(true);
                        Debug.Log("Cerrando sesión");
                        SceneManager.LoadScene("MenuPrincipal");
                    }
                    else if (mensaje == "Error al desconectar")
                        notificacion.text = "Error al desconectar";
                    break;
            }
        }
    }

    public void IniciarSesion() //procedimiento para iniciar sesion
    {
        string Name = NameInput.text;
        string Password = PasswordInput.text;

        if ((Password != "") || Name != "")
        {
            string mensajeIniciarSesion = "0-" + Name + "-" + Password;
            conexionServidor.EnviarMensajeServidor(mensajeIniciarSesion);
            Debug.Log("Enviado");
            usuario = NameInput.text;
            contrasena = PasswordInput.text;
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
                usuario = NameInput.text;
                contrasena = PasswordInput.text;
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

    public void DameTiempoJugado() //consulta 1 para buscar el tiempo total jugado de un jugador por su nombre
    {
        string NameJugadorInputField = NombreJugadorInputField.text;

        string query1 = "2-" + NameJugadorInputField;
        conexionServidor.EnviarMensajeServidor(query1);
        Debug.Log("Enviado");
    }
    public void DameCircuitoFav() //consulta 2 para encontrar el circuito favorito de un jugador por su nombre
    {
        string NameJugadorInputField = NJInputField.text;

        string query2 = "3-" + NameJugadorInputField;
        conexionServidor.EnviarMensajeServidor(query2);
        Debug.Log("Enviado");
    }
    public void DameDuracionCircuito() //consulta 3 para encontrar cual es la duracion total de un circuito
    {
        string NameCircuitInputField = NombreCircuitoInputField.text;

        string query3 = "4-" + NameCircuitInputField;
        conexionServidor.EnviarMensajeServidor(query3);
        Debug.Log("Enviado");
    }
    public void DameConectados()
    {
        string conectados = "5";
        conexionServidor.EnviarMensajeServidor(conectados);
        Debug.Log("Enviado");
    }
     public void Desconectar() //desconectar/log out
    {
        string mensaje = "6-" + usuario + "-" + contrasena;
        conexionServidor.EnviarMensajeServidor(mensaje);
        Debug.Log("Enviado");
    }

    public void Cerrar() //cerrar el juego desde el menu
    {
        conexionServidor.Desconectarse();
        Application.Quit();
    }
}

