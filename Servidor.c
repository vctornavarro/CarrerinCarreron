#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <mysql.h>

int main (int argc, char *argv[])
{
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
	char respuesta1[512];
	//abrimos socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creando socket");
	//hacemos el bind al puerto
	//Inicaliza en zero serv_adr
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	
	//Asocia el socket a cualquier IP de la maquina
	//htonl formatea el numero que recibe al formato necessario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	//escuchamos en el puerto 9050
	serv_adr.sin_port = htons(9050);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	
	//Maximo de peticiones en la cola es de 3
	if (listen(sock_listen, 3) < 0)
		printf ("Error en el Linsten");
	
	int i;
	//Atenderemos 3 peticiones
	for (;;){
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion\n");
		int terminar = 0;
		while(terminar == 0)
		{
			//Ahora recibimos su nombre, que dejamos en el buf
			ret=read(sock_conn,peticion, sizeof(peticion));
			printf ("Recibido\n");
			
			//Tenemos que añadir la marca de fin de string para que no escriba lo que hay en el buffer
			peticion[ret]='\0';
			
			//Escribinos el nombre en consola
			
			printf("Peticion: %s\n", peticion);
			
			//Vamos a ver que nos pide la peticion
			char *p = strtok( peticion, "/");
			int codigo;
			codigo = atoi(p);
			char NOMBRE[50];
			int id;
			char circuito[20];
			char PASS[15];
			if (codigo !=0)
			{
				p = strtok (NULL, "/");
				
				strcpy(NOMBRE, p);
				printf ("ID: %s, Nombre: %s\n", id, NOMBRE);
			}
			if (codigo ==0) //peticion de desconexion
			{
				terminar = 1;
			}
			else if(codigo == 1)
			{
				p = strtok (NULL, "/");
				strcpy(PASS, p);
				printf ("ID: %s, Nombre: %s, Password: %s\n", id, NOMBRE, PASS);
				
				MYSQL *conn;
				int err;
				
				MYSQL_RES *resultado;
				MYSQL_ROW row;
				
				char consulta [100];
				
				conn = mysql_init(NULL);
				if (conn==NULL) {
					printf ("Error al crear la conexion: %u %s\n", 
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				
				conn = mysql_real_connect (conn, "localhost","root", "mysql", "Carrerin Carreron",0, NULL, 0);
				if (conn==NULL) {
					printf ("Error al inicializar la conexion: %u %s\n", 
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				
				sprintf(consulta, "SELECT JUGADOR.NOMBRE, JUGADOR.PASS FROM JUGADOR WHERE (JUGADOR.NOMBRE='%s' AND JUGADOR.PASS='%s')", NOMBRE, PASS);
				
				err=mysql_query (conn, consulta);
				if (err!=0) {
					printf ("Error al consultar datos de la base %u %s\n",
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				printf("aqui\n");
				resultado = mysql_store_result (conn);
				
				printf("aqui\n",resultado);
				
				row = mysql_fetch_row (resultado);
				printf(row[0]);
				
				
				if (row[0] == NULL)
				{
					printf ("No se han obtenido datos en la consulta\n");
					strcpy(respuesta1, "NO");
				}
				
				else
				{	
					strcpy(respuesta1, "SI");
				}
				
				write (sock_conn, respuesta1, strlen(respuesta1));
				
				mysql_close (conn);
				
			}
			else if(codigo == 2)
			{
				printf ("Codigo: %d, Id: %s\n", codigo, id);
				sprintf (respuesta,"%d",DameTiempoJugador(id));
				write (sock_conn, respuesta, strlen(respuesta));
			}
			else if(codigo == 3)
			{
				printf ("Codigo: %d, Nombre: %s\n", codigo, NOMBRE);
				sprintf (respuesta,"%d",DameCircuitoFav(NOMBRE));
				write (sock_conn, respuesta, strlen(respuesta));
			}
			else if(codigo == 4)
			{
				printf ("Codigo: %d, Circuito: %s\n", codigo, circuito);
				sprintf (respuesta,"%d",DameDistancia(circuito));
				write (sock_conn, respuesta, strlen(respuesta));
			}
			printf ("Respuesta: %s\n", respuesta);
			//lo enviamos
		}
		close(sock_conn); 
	}
}
//Dame el tiempo jugado de un jugador por su ID
void DameTiempoJugador (int id)
{
	MYSQL *conn;
	int err;
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	
	char consulta [100];
	
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexin
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "Carrerin Carreron",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	//consulta SQL
	strcpy (consulta, "SELECT TIEMPOTOTALJUGADO FROM JUGADOR WHERE JUGADOR.ID = '");
	strcat (consulta, id);
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
	char tiempototal [20];
	if(row==NULL)
		printf ("No se han obtenido datos\n");
	else	
	{
		strcpy(tiempototal, row[0]);
		while ((row = mysql_fetch_row(resultado)) != NULL){
			strcat(tiempototal, "-");
			strcat(tiempototal, row[0]);
		}
	printf("Tiempo total: %s\n", tiempototal);
	
	
	mysql_close (conn);
}}
//Circuito favorito de un jugador por su nombre
void DameCircuitoFav(char NOMBRE[50])
{
	MYSQL *conn;
	int err;
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	
	char consulta [100];
	
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexion
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "Carrerin Carreron",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	//consulta SQL
	strcpy (consulta, "SELECT CIRCUITOFAV FROM JUGADOR WHERE JUGADOR.NOMBRE = '");
	strcat (consulta, NOMBRE);
	strcat (consulta, "'");
	
	//Para comprobar errores en la consulta
	err=mysql_query(conn, consulta);
	if(err!=0){
		printf("Error al consultar los datos de la base %u%s\n", mysql_errno(conn),mysql_error(conn));
		exit(1);
	}
	
	resultado=mysql_store_result(conn);
	row =mysql_fetch_row(resultado);
	
	char circuitofav [20];
	if(row==NULL){
		printf ("No se han obtenido datos\n");
	}
	else
	{
		strcpy(circuitofav, row[0]);
		while ((row = mysql_fetch_row(resultado)) != NULL){
			strcat(circuitofav, "-");
			strcat(circuitofav, row[0]);
		}
	printf("Su circuito favorito es: %s\n", circuitofav);
	
	mysql_close (conn);
	}}
//Dame la distancia de un circuito
void DameDistancia(char circuito[20])
{
	MYSQL *conn;
	int err;
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	
	char consulta [100];
	
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexion
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "Carrerin Carreron",0, NULL, 0);
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
	err=mysql_query(conn, consulta);
	if(err!=0){
		printf("Error al consultar los datos de la base %u%s\n", mysql_errno(conn),mysql_error(conn));
		exit(1);
	}
	resultado=mysql_store_result(conn);
	
	row =mysql_fetch_row(resultado);
	int distancia;
	if(row==NULL)
		printf ("No se han obtenido datos\n");
	else {
		distancia = row[0];
		while(row!=NULL){
		printf("La distancia del circuito son: %s metros\n", distancia);
	}
		mysql_close (conn);
	}
}
