#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>
#include <mysql.h>
#include <arpa/inet.h>

//Estructura necessaria para login excluyente
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#define MAX_CLIENTS 4
MYSQL* conn;
MYSQL_RES* resultado;
MYSQL_ROW row;
int err;

typedef struct {
	char nombre[20];
	int socket;
}Conectado;

typedef struct {
	Conectado conectados[100];
	int num;
} ListaConectados;

typedef struct {
	int idP;
	char usrHost[20];
	int socketHost;
	char usrInvitado[20];
	int socketInvitado;
}Partida;

ListaConectados lista;

int i = 0;
int sockets[100];

int Conectar(ListaConectados* lista, char nombre[20], int socket);

int PosicionJugador(ListaConectados* lista, char nombre[20])
{
	int i = 0;
	int encontrado = 0;
	while ((i < lista->num) && !encontrado)
	{
		if (strcmp(lista->conectados[i].nombre, nombre) == 0)
		{
			encontrado = 1;
		}
		else
			i++;
	}
	if (encontrado)
		return i;
	else
		return -1;
}


void DameConectados(ListaConectados* lista, char conectados[300])
{
	int i;
	char temp[300];
	
	sprintf(conectados, "5-%d", lista->num);
	
	for (i = 0; i < lista->num; i++)
	{
		sprintf(temp, "-%s", lista->conectados[i].nombre);
		strcat(conectados, temp);
	}
}

int AddConectado(ListaConectados* lista, char nombre[20], int socket)
{
	if (lista->num == 100)
	{
		return -1;
	}
	else
	{
		strcpy(lista->conectados[lista->num].nombre, nombre);
		lista->conectados[lista->num].socket = socket;
		lista->num++;
		return 0;
	}
	
}
int DamePos(ListaConectados* lista, char nombre[20])
{
	int i = 0;
	int terminado = 0;
	int pos;
	
	if (lista->conectados[i].nombre == nombre)
	{
		int pos = i;
	}
	return pos;
}


int Desconectar(ListaConectados* lista, char nombre[20])//desconecta de la lista de conectados al usuario
{
	int pos = PosicionJugador(lista, nombre);
	if (pos == -1)
		return -1;
	else
	{
		int i;
		for (i = pos; i < lista->num - 1; i++)
		{
			lista->conectados[i] = lista->conectados[i + 1];
		}
		lista->num--;
		return 0;
	}
}

int Conectar(ListaConectados* lista, char nombre[20], int socket)//a\uffc3\uffaf\uffc2\uffbf\uffc2\uffb1ade a la lista de conectados al usuario
{
	if (lista->num == 100)
		return -1;
	else
	{
		strcpy(lista->conectados[lista->num].nombre, nombre);
		lista->conectados[lista->num].socket = socket;
		lista->num++;
		
		return 0;
	}
}
void Login(char nombre[25], char contrasena[25], char respuesta[512])
{
	
	int login;
	char consulta[500];
	
	conn = mysql_init(NULL);
	if (conn == NULL) {
		printf("Error al crear la conexion1: %u %s\n",
			   mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	
	conn = mysql_real_connect(conn, "localhost", "root", "mysql", "M2_bd", 0, NULL, 0);
	if (conn == NULL)
	{
		printf("Error al inicializar la conexion2: %u %s\n",
			   mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	
	sprintf(consulta, "SELECT JUGADOR.NOMBRE, JUGADOR.PASS FROM JUGADOR WHERE (JUGADOR.NOMBRE='%s' AND JUGADOR.PASS='%s')", nombre, contrasena);
	err = mysql_query(conn, consulta);
	if (err != 0)
	{
		printf("Error al consultar datos de la base %u %s\n",
			   mysql_errno(conn), mysql_error(conn));
		exit(1);
		sprintf(respuesta, "0-Error-Al consultar la base de datos");
	}
	else
	{
		MYSQL_RES* resultado;
		resultado = mysql_store_result(conn);
		int num_filas = mysql_num_rows(resultado);
		mysql_free_result(resultado);
		
		if (num_filas > 0) {
			printf("Ha iniciado sesion el usuario con nombre: %s\n", nombre);
			sprintf(respuesta, "0-SI");
		}
		else {
			printf("No se encontro el usuario con nombre: %s\n", nombre);
			sprintf(respuesta, "0-Error-Usuario o contrasena mal introducida");
		}
	}
	mysql_close(conn);
}


void Registrar(char nombre[25], char contrasena[25], char respuesta[512])
{
	char consulta[500];
	int numJ;
	
	conn = mysql_init(NULL);
	if (conn == NULL) {
		printf("Error al crear la conexion1: %u %s\n",
			   mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	
	conn = mysql_real_connect(conn, "localhost", "root", "mysql", "M2_bd", 0, NULL, 0);
	if (conn == NULL)
	{
		printf("Error al inicializar la conexion2: %u %s\n",
			   mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	
	sprintf(consulta, "INSERT INTO JUGADOR VALUES ('%s', '%s', 0, 0, 0, 0)", nombre, contrasena);
	err = mysql_query(conn, consulta);
	if (err != 0)
	{
		printf("Error al insertar datos en la base: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
		sprintf(respuesta, "1-Error");
	}
	
	else
	{
		sprintf(respuesta, "1-SI");
	}
	mysql_close(conn);
}

void DameTiempoJugador(char nombre[20], char respuesta[512])
{
	char tiempo[20];
	char consulta [100];
	
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexin
	conn = mysql_real_connect(conn, "localhost", "root", "mysql", "M2_bd", 0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	//consulta SQL
	strcpy (consulta, "SELECT TIEMPOTOTALJUGADO FROM JUGADOR WHERE JUGADOR.NOMBRE = '");
	strcat (consulta, nombre);
	strcat (consulta, "'");
	
	//Para comprobar errores en la consulta
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	//recogemos el resultado de la consulta
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	
	//Recogemos el resultado
	if(row == NULL || row[0] == NULL){
		printf ("No se han obtenido datos\n");
		sprintf(respuesta, "2-Error");
	}
	else	
	{
		strcpy(tiempo, row[0]);
		sprintf(respuesta, "2-SI-%s", tiempo);
	}
	mysql_close (conn);
}
	//Circuito favorito de un jugador por su nombre
void DameCircuitoFav(char nombre[50], char respuesta[512])
	{
	char circuito[20];
	char consulta [100];
	
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexin
	conn = mysql_real_connect(conn, "localhost", "root", "mysql", "M2_bd", 0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	//consulta SQL
	strcpy (consulta, "SELECT CIRCUITOFAV FROM JUGADOR WHERE JUGADOR.NOMBRE = '");
	strcat (consulta, nombre);
	strcat (consulta, "'");
	
	//Para comprobar errores en la consulta
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	//recogemos el resultado de la consulta
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	
	//Recogemos el resultado
	if(row == NULL || row[0] == NULL){
		printf ("No se han obtenido datos\n");
		sprintf(respuesta, "3-Error");
	}
	else	
	{
		strcpy(circuito, row[0]);
		sprintf(respuesta, "3-SI-%s", circuito);
	}
	mysql_close (conn);
	}
//Dame la distancia de un circuito
void DameDistancia(char circuito[20], char respuesta[512])
	{
	int distancia;
	char consulta [100];
	
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexin
	conn = mysql_real_connect(conn, "localhost", "root", "mysql", "M2_bd", 0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	//consulta SQL
	strcpy (consulta, "SELECT DISTANCIA FROM CARRERIN WHERE CARRERIN.CIRCUITO = '");
	strcat (consulta, circuito);
	strcat (consulta, "'");
	
	//Para comprobar errores en la consulta
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	//recogemos el resultado de la consulta
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	
	//Recogemos el resultado
	if(row == NULL || row[0] == NULL){
		printf ("No se han obtenido datos\n");
		sprintf(respuesta, "4-Error");
	}
	else	
	{
		distancia = atoi(row[0]);
		sprintf(respuesta, "4-SI-%d", distancia);
	}
	mysql_close (conn);
}
		
void* atenderCliente(void* socket)
{
	int ret;
	int sock_conn;
	int* s;
	s = (int*)socket;
	sock_conn = *s;
	
	char peticion[512];
	char respuesta[512];
	char contestacion[512];
	char contrasena[20];
	char consultas[512];
	char numero;
	
	char conectados[300];
	int conexion = 0;
	int r;
	int i;
	char NOMBRE[20];
	char NOMBRE2[20];
	char circuito[20];
	char id[20];
	
	while (conexion == 0)
	{
		ret = read(sock_conn, peticion, sizeof(peticion));
		printf("Recibido\n");
		peticion[ret] = '\0';
		int error = 1;
		int codigo = 9999;
		printf("Peticion: %s\n", peticion);
		char* p = strtok(peticion, "-");
		codigo = atoi(p);
		
			if ((codigo != 2) && (codigo != 3)&&(codigo!=4)&&(codigo!=9)) {
			p = strtok(NULL, "-");
			if (p != NULL) {
				strcpy(NOMBRE, p);
				printf("Codigo: %d, Nombre: %s\n", codigo, NOMBRE);
			}
			}	
			if (codigo == 0) //LOGIN
            {
                p = strtok(NULL, "-");
                if (p != NULL) {
                    strcpy(contrasena, p);
                    printf("Codigo: %d, Nombre: %s y Contrasena: %s\n", codigo, NOMBRE, contrasena);
                    Login(NOMBRE, contrasena, contestacion);
                    pthread_mutex_lock(&mutex);
                    if (strcmp(contestacion, "Error") != 0) {
                        r = Conectar(&lista, NOMBRE, socket);
                        DameConectados(&lista, conectados);
                        sprintf(respuesta, "%s", contestacion);
                        write(sock_conn, respuesta, strlen(respuesta));
                        int j;
                        for (j = 0; j < lista.num; j++)
                        {
                        sprintf(respuesta, "%s", conectados);
                        write(sockets[j], respuesta, strlen(respuesta));
                        }
                    }
                    pthread_mutex_unlock(&mutex);
                }
            }
			else if (codigo == 1) //REGISTRAR
            {

                p = strtok(NULL, "-");
                if (p != NULL) {
                    strcpy(contrasena, p);
                    printf("Codigo: %d, Nombre: %s y Contrasena: %s\n", codigo, NOMBRE, contrasena);
                    Registrar(NOMBRE, contrasena, contestacion);
                    pthread_mutex_lock(&mutex);
                    if (strcmp(contestacion, "Error") != 0) {
                        r = Conectar(&lista, NOMBRE, socket);
                        DameConectados(&lista, conectados);
                        sprintf(respuesta, "%s", contestacion);
                        write(sock_conn, respuesta, strlen(respuesta));
                        int j;
                        for (j = 0; j < lista.num; j++)
                        {
                        sprintf(respuesta, "%s", conectados);
                        write(sockets[j], respuesta, strlen(respuesta));
                        }
                    }
                    pthread_mutex_unlock(&mutex);
                }
            }
			else if(codigo == 2)
			{
				p = strtok(NULL, "-");
				if (p != NULL) {
					strcpy(consultas, p);
					printf("Codigo: %d, Nombre: %s\n", codigo, consultas);
					DameTiempoJugador(consultas, contestacion);
					sprintf(respuesta, "%s", contestacion);
					write(sock_conn, respuesta, strlen(respuesta));
				}
			}
			else if(codigo == 3)
			{
				p = strtok(NULL, "-");
				if (p != NULL) {
					strcpy(consultas, p);
					printf("Codigo: %d, Nombre: %s\n", codigo, consultas);
					DameCircuitoFav(consultas, contestacion);
					sprintf(respuesta, "%s", contestacion);
					write(sock_conn, respuesta, strlen(respuesta));
				}
			}
			else if(codigo == 4)
			{
				p = strtok(NULL, "-");
				if (p != NULL) {
					strcpy(consultas, p);
					printf("Codigo: %d, Circuito: %s\n", codigo, consultas);
					DameDistancia(consultas, contestacion);
					sprintf(respuesta, "%s", contestacion);
					write(sock_conn, respuesta, strlen(respuesta));
				}
			}
			else if(codigo == 5)
			{
				printf("Codigo: %d\n", codigo);
				pthread_mutex_lock(&mutex);
				DameConectados(&lista, contestacion);
				sprintf(respuesta, "%s", contestacion);
				write(sock_conn, respuesta, strlen(respuesta));
				pthread_mutex_unlock(&mutex);
			}
			else if (codigo == 6) //DESCONECTAR/LOGOUT
            {
                pthread_mutex_lock(&mutex);
                printf("Desconectando a %s\n", NOMBRE);
                r = Desconectar(&lista, NOMBRE);
                printf("Codigo de desconexion: %d\n", r);
                if (r == 0)
                {
                    DameConectados(&lista, conectados);
                    sprintf(respuesta, "6-Desconectando");
                    write(sock_conn, respuesta, strlen(respuesta));
                    int j;
                    for (j = 0; j < lista.num; j++)
                    {
                        sprintf(respuesta, "%s", conectados);
                        write(sockets[j], respuesta, strlen(respuesta));
                    }
                }
                else if (r == -1)
                {
                    sprintf(respuesta, "6-Error al desconectar");
                    write(sock_conn, respuesta, strlen(respuesta));
                }
                pthread_mutex_unlock(&mutex);
            }
			else if (codigo == 7) //reenviar invitacion
			{
				p = strtok(NULL, "-");
				if (p != NULL) {
					strcpy(NOMBRE2, p);
					if (strcmp(NOMBRE, NOMBRE2) != 0)
					{
						int PosicionParaInvitar = PosicionJugador(&lista, NOMBRE2);
						if (PosicionParaInvitar == -1) {
							sprintf(respuesta, "7-No existe");
							write(sock_conn, respuesta, strlen(respuesta));
						}
						else
						{
							sprintf(respuesta, "8-%s", NOMBRE);
							write(lista.conectados[PosicionParaInvitar].socket, respuesta, strlen(respuesta));
							sprintf(respuesta, "7-Invitacion enviada");
							write(sock_conn, respuesta, strlen(respuesta));
						}
					}
					else if (strcmp(NOMBRE, NOMBRE2) == 0) {
						sprintf(respuesta, "7-No puedes invitarte a ti mismo");
						write(sock_conn, respuesta, strlen(respuesta));
					}
				}
			}
			/*else if (codigo == 8) //respuesta del invitado al host
			{
				char respuestaInvitacion[20];
				char usuarioHost[20];
				p = strtok(NULL, "-");
				strcpy(respuestaInvitacion, p);
				p = strtok(NULL, "-");
				strcpy(usuarioHost, p);
				int PosicionParaRespuesta = PosicionJugador(&lista, usuarioHost);
				if (strcmp(respuestaInvitacion, "SI") == 0)
				{
					strcpy(NOMBRE, usuarioHost);
					int addPartida = AddPartida(NOMBRE, NOMBRE2);
					int currentmatchID = GetIDUltimaPartida(conn);
					sprintf(respuesta, "9-Invitacion aceptada-%d", currentmatchID);
					write(lista.conectados[PosicionParaRespuesta].socket, respuesta, strlen(respuesta));
				}
				else if (strcmp(respuestaInvitacion, "NO") == 0)
				{
					sprintf(respuesta, "9-Invitacion rechazada");
					write(lista.conectados[PosicionParaRespuesta].socket, respuesta, strlen(respuesta));
				}
			}*/
		
		else if (codigo == 9) 
		{ //reenviar mensaje
			char mensaje[200];
			p = strtok(NULL, "-");
			if (p != NULL) 
			{
				strcpy(mensaje, p);
				char usuario[200];
				p = strtok(NULL, "-");
				if (p != NULL) 
				{
					strcpy(usuario, p);
					sprintf(respuesta, "10-%s-%s", mensaje, usuario);
					printf("Mensaje: %s\n", respuesta);
					int j;
					for (j = 0; j < lista.num; j++)
					{
						write(sockets[j], respuesta, strlen(respuesta));
					}
				}
			}
		}
	}

		close(sock_conn); 
	}



int main(int argc, char* argv[])
{
	int sock_conn, sock_listen;
	struct sockaddr_in serv_adr;
	
	pthread_t thread;
	lista.num = 0;
	int conexion = 0;
	int puerto = 5030; //4,5,6
	int i = 0;
	
	//abrimos el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Error al crear socket\n");
		exit(EXIT_FAILURE);
	}
	//Bind en el puerto
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	//establecemos puerto de escucha
	serv_adr.sin_port = htons(puerto);
	if (bind(sock_listen, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) < 0) {
		printf("Error en el bind\n");
		exit(EXIT_FAILURE);
	}
	
	if (listen(sock_listen, 3) < 0) {
		printf("Error en el Listen\n");
		exit(EXIT_FAILURE);
	}
	
	while (conexion == 0)
	{
		printf("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf("Conexion recibida\n");
		
		
		sockets[i] = sock_conn;
		
		pthread_create(&thread, NULL, atenderCliente, &sockets[i]);
		
		i++;
	}
	return 0;
	
}

