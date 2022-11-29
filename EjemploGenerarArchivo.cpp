#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>

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
        cout << (buffer[SIZE]) << "Datos";     
    }

    cout << buffer;
    return;
}

int main(){

    char *ip = "127.0.0.1";
    //int puerto = 8080;
    int unir;

    int sockfd, new_sock;

    socklen_t addr_size;
    char buffer[4096];

    //Se crea el socketaa
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
    if(sockfd < 0){
        perror("[-]ERROR IN SOCKET");
        exit(1);
    }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   

    printf("El socket se ha conectado satisfactoriamente. \n");

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(puerto);
    inet_pton(AF_INET, ip, &hint.sin_addr);

    unir = bind(sockfd, (sockaddr*)&hint, sizeof(hint));

    if (unir < 0){
        perror("[ERROR AL UNIR]");
        exit(1);
    }
    printf("[+]Asociacion entre la direccion ip y el puerto completa. \n");

    if(listen(sockfd, 10) == 0){
        printf("[+]Esperando conexion...\n");

    }else{
        perror("[-]Error al escuchar");
        exit(1);
    }
    
    sockaddr_in new_addr;
    addr_size = sizeof(new_addr);

    new_sock = accept(sockfd, (sockaddr*)&new_addr, &addr_size);

    //int n = recv(sockfd, buffer, SIZE, 0);
    //cout << buffer;
    
    //write_file(new_sock);
    printf("[+]Informacion recibida exitosamente");

    return 0;
}