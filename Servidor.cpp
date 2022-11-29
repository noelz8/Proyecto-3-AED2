#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

#include<iostream>
#include<mysql/mysql.h>
#include<stdio.h>

#include <cmath>

using namespace std;
#define SIZE 1024
#define puerto 8080

#define SERVER "localhost"
#define USER "root"
#define PASSWORD "123"
#define DATABASE "db_pruebas"

/**
* @brief Funciones del Servidor de aquí en adelante 
* 
* 
*/

//Funcion para escribir en el archivo
void write_file(int sockfd){

    int n;
    FILE *fp;
    char *filename = "Recibido.txt";
    char buffer[SIZE];

    fp = fopen(filename, "w");
    while(1){
        n = recv(sockfd, buffer, SIZE, 0);
        if (n <= 0){
            break;
            return;
        }
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);       
    }
    return;
}

int main()
{
    //Variables necesarias

    //Conexion cliente servidor
    char *ip = "127.0.0.1";

    int unir;
    int sockfd, new_sock;
    socklen_t addr_size;

    //conexion mysql
    MYSQL *connect;
    connect=mysql_init(NULL);

    //Guardar los datos
    char buf[SIZE];
    char buf2[SIZE];
    char fact[4096];

    string comandostring = "";
    string ParametroString = "";

    //Se crea la conexion a Mysql
    if (!connect)
    {
    cout<<"MySQL Initialization failed";
    return 1;
    }

    connect=mysql_real_connect(connect, "localhost", "root", "123" , "db_pruebas", 0, NULL, 0);

    if (connect)
    {
    cout<<"connection Succeeded\n";
    }
    else
    {
    cout<<"connection failed\n";
    }

    // Variables necesarias para la conexión y contar las columnas mysql
    MYSQL_RES *res_set;
    MYSQL_ROW row;

    // Se crea el socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        cerr << "Cant create a socket";
        return -1;
    }
    printf("El socket se ha conectado satisfactoriamente. \n");

    // Se asignan valores importantes para el socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(puerto);
    inet_pton(AF_INET, ip, &hint.sin_addr);

    bind(listening, (sockaddr*)&hint, sizeof(hint));
 
    // Esuchando para una posible conexion
    listen(listening, SOMAXCONN);

    if(listen(listening, 10) == 0){
        printf("[+]Esperando conexion...\n");
    }else{
        perror("[-]Error al escuchar");
        exit(1);
    }
 
    // Esperando una conexion
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
 
    // Se conecta el socket con el cliente
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
 
    char host[NI_MAXHOST];      
    char service[NI_MAXSERV];   
 
    memset(host, 0, NI_MAXHOST); 
    memset(service, 0, NI_MAXSERV);
 
    // Se obtiene la informacion de conexion del cliente
    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        cout << host << " connected on port " << service << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on port " << ntohs(client.sin_port) << endl;
    }
 
    // Se cierra el socket en escucha
    close(listening);
    
    char *path;
    //Este while recibe los datos del cliente y manda lo necesario
    while (true)
    {   
        //
        memset(buf, 0, SIZE);
        memset(buf2, 0, SIZE);
        memset(fact, 0, 4096);
    
        char *err = "Error al encontrar el dato enviado por el cliente";

        // Esperando a que el cliente envie el mensaje
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == -1){
            cerr << "Error in recv(). Quitting" << endl;
            break;
        }
        //En caso de error
        if (bytesReceived == 0){
            cout << "Client disconnected " << endl;
            break;
        }
        //Le pasamos el comando char a un string global
        comandostring = buf;
        
        //Envio de mensaje al cliente 
        char *mensaje = "Comando aceptado exitosamente";
        send(clientSocket, mensaje, strlen(mensaje) + 1, 0);

        //Recibir el parametro desde el cliente
        int bytes2 = recv(clientSocket, buf2, 4096, 0);
        if (bytes2 == -1){ 
            cerr << "Error al recibir" << endl;
            break;
        }
        //En caso de error
        if (bytes2 == 0){
            cout << "Cliente desconectado " << endl;  
        }

        //Le pasamos el comando char a un string global
        ParametroString = buf2;

        /**
         * @brief Funciones del help
         */

        if (comandostring == "help"){
            cout << comandostring << "\n Datos de comandos y ayudas \n";

            //mysql_query (connect,"INSERT INTO tb_help (COMANDO, DESCRIC) VALUES ('help', 'Muestra la ayuda'), ('add', 'añade un archivo');");
            mysql_query (connect,"select * from tb_help;");
            unsigned int i =0;
            res_set = mysql_store_result(connect);
            unsigned int numrows = mysql_num_rows(res_set);

            cout << endl;
            cout <<" ---------------------------------------------------------------- \t"<< endl;

            while (((row= mysql_fetch_row(res_set)) !=NULL ))
            { 
            cout <<" | \t" << row[i] << "\t | \t"<<  row[i+1] << "\t | \t"<< row[i+2] << "\t | \t" << endl;
            cout <<" ---------------------------------------------------------------- \t"<< endl;
            }

            mysql_close (connect);
            //Envio de mensaje al cliente s
            char *mensaje2 = "Los comando de ayuda se desplegaron correctamente";
            send(clientSocket, mensaje2, strlen(mensaje2) + 1, 0);  
        }
    }

    //Finaliza la conexión
    close(clientSocket);
    return 0;
}