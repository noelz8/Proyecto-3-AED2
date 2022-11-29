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

#include <cmath>

using namespace std;
#define SIZE 1024
#define puerto 8080

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
    //cout << "(" << buffer << ")";
    //cout << "***";
    return;
}

int main()
{
    //Variables necesarias
    char *ip = "127.0.0.1";

    int unir;

    int sockfd, new_sock;

    socklen_t addr_size;

    char buf[SIZE];
    char buf2[SIZE];
    char fact[4096];

    string comandostring = "";
    string segundoDato = "";

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
    while (true)
    {
        
        memset(buf, 0, SIZE);
        memset(buf2, 0, SIZE);
        memset(fact, 0, 4096);
    
        char *err = "Error al encontrar el dato enviado por el cliente";

        // Esperando a que el cliente envie el mensaje
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == -1)
        {
            cerr << "Error in recv(). Quitting" << endl;
            break;
        }
 
        if (bytesReceived == 0)
        {
            cout << "Client disconnected " << endl;
            break;
        }
        cout << buf << "**1\n";
        comandostring = buf;
        
        char *mensaje = "Comando aceptado exitosamente";

        send(clientSocket, mensaje, strlen(mensaje) + 1, 0);


        int bytes2 = recv(clientSocket, buf2, 4096, 0);
        if (bytes2 == -1){
            
            cerr << "Error al recibir" << endl;
            break;
        }

        if (bytes2 == 0){
            
            cout << "Cliente desconectado " << endl;
            
        }

        cout << buf2 << "**2\n";
        segundoDato = buf2;

        char *mensaje2 = "Parametro aceptado exitosamente";

        send(clientSocket, mensaje2, strlen(mensaje2) + 1, 0);

        /*char *men = "Hola";
        
        send(clientSocket, men, strlen(men) + 1, 0);

        int bytes2 = recv(clientSocket, buf2, 4096, 0);

        if (bytes2 == -1){
            
            cerr << "Error al recibir" << endl;
            break;
        }

        if (bytes2 == 0){
            
            cout << "Cliente desconectado " << endl;
            
        }

        if(string(buf2, 0, bytes2)=="1"){

            image.gaussianBlur(buf);
            break;
        }

        if(string(buf2, 0, bytes2)=="2"){

            image.gray_scale(buf);
            break;
        }

        if(string(buf2, 0, bytes2)=="3"){

            send(clientSocket, factor, strlen(factor)+1, 0);
            int facts = recv(clientSocket, fact, 4096, 0);
            image.brightControl(buf, stoi(fact));
            break;
            


        }

        if (string(buf2, 0, bytes2)=="4"){

            send(clientSocket, gamma, strlen(gamma)+1, 0);
            int facts = recv(clientSocket, fact, 4096, 0);
            image.gammaCorrection(buf, stoi(fact));
            break;
        }*/
   
    }
    //cout << completString << "**2\n";
    // Close the socket
    close(clientSocket);

    //cout << completString << "**3\n";
 

    return 0;
}