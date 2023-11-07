using System.Net;
using System.Net.Sockets;
using System.Text;
using UnityEngine;

namespace Client
{
    /** Singleton class para conectarse al servidor a trav�s del socket */
    public class ConexionServidor
    {
        private Socket servidor;
        private IPAddress ip;
        private IPEndPoint port;
        private bool connected = false;
        private bool loggedIn = false;
        private static ConexionServidor instance = null;
        public int puerto = 5070;


        // Constructor privado para evitar que se instancie directamente
        public ConexionServidor()
        {
            this.ip = IPAddress.Parse("192.168.56.102");
            this.port = new IPEndPoint(this.ip, puerto);
            servidor = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        }

        // Obtener la instancia �nica de la conexi�n al servidor
        public static ConexionServidor GetInstance()
        {
            if (instance == null)
            {
                instance = new ConexionServidor();
            }
            return instance;
        }

        // Funci�n para conectarse al servidor
        public int ConnectToServer()
        {
            if (connected)
            {
                Debug.Log("Already connected to server");
                return 0;
            }

            Debug.Log("Connecting to server");
            try
            {
                servidor.Connect(port); // Intentamos conectar el socket     
                connected = true;
                Debug.Log("Connected");
                return 0;
            }
            catch (SocketException)
            {
                Debug.Log("Server Connection Error");
                return -1;
            }
        }

        // Funci�n que escucha para recibir mensajes del servidor
        public string EscucharMensaje()
        {
            byte[] mensajeRecibido = new byte[1024];
            int tamañoDatos = servidor.Receive(mensajeRecibido);
            string msg = Encoding.ASCII.GetString(mensajeRecibido, 0, tamañoDatos);
            string mensajelimpio = msg.Split('\0')[0];
            return mensajelimpio;
        }
        public void EnviarMensajeServidor(string mensaje) //funcion para enviar mensajes al servidor

        {
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            servidor.Send(msg);
        }

        // Funci�n para cerrar la conexi�n
        public void Desconectarse() //procedimiento para desconectarse del servidor
        {
            // Enviar c�digo de desconexi�n al servidor
            string mensaje = "5-";
            EnviarMensajeServidor(mensaje);
            servidor.Shutdown(SocketShutdown.Both);
            servidor.Disconnect(true);
            servidor.Close();
        }

        // Getter para la propiedad connected
        public bool IsConnected()
        {
            return connected;
        }

        // Getter y setter para la propiedad loggedIn
        public bool IsLoggedIn()
        {
            return loggedIn;
        }

        public void SetLoggedIn(bool loggedIn)
        {
            this.loggedIn = loggedIn;
        }
    }
}