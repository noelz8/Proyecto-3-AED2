#include <stdio.h>
#include "iostream"
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#define SIZE 1024

using namespace std;

//Funcion que envia el archivo.
void send_file(FILE *fp, int sockfd){

    int n;
    char data[SIZE] = {0};

    while(fgets(data, SIZE, fp) != NULL){
        if(send(sockfd, data, sizeof(data), 0) == -1){
            perror("[-]Error al enviar el archivo");
            exit(1);
        }
        bzero(data, SIZE);
    }
} 

string filename1;

int main(){
    char *ip = "127.0.0.1";
    int puerto = 8080;

    int coneccion;

    int sockfd;
    FILE *fp;
    char filename[SIZE];
    printf("Ingrese la ubicacion del archivo a enviar.\n");
    getline(cin, filename1);
    strcpy(filename, filename1.c_str());



    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        perror("[-]Error en socket");
        exit(1);
    }
    printf("[+]Socket creado correctamente \n");

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(puerto);
    inet_pton(AF_INET, ip, &hint.sin_addr);
    



    coneccion = connect(sockfd, (struct sockaddr*)&hint, sizeof(hint));
    if(coneccion == -1){
        perror("[-]Error en el socket");
        exit(1);
    }
    printf("[+]Conectando al servidor\n");


    fp = fopen(filename, "r");
    if (fp == NULL){
        perror("[-]Error al abrir el archivo");
        exit(1);
    }

    send_file(fp, sockfd);
    printf("[+]Archivo enviado.\n");

    printf("[+]Cerrando conexion.\n");
   

    return 0;

}