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

string dataFull;

string filename1;


int main(){
    char *ip = "127.0.0.1";
    int puerto = 8080;

    int coneccion;

    int sockfd;
    FILE *fp;
    char filename[SIZE];
    char palab[SIZE];

    string palabra;

    printf("Ingrese la ubicacion del archivo a enviar.\n");
    getline(cin, filename1);
    strcpy(filename, filename1.c_str());
    printf("Ingrese un apalabra\n");
    getline(cin, palabra);
    //strcpy(palab, palabra.c_str());


    cout << "nueva : " << palabra;
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

    int n;
    char data[SIZE] = {0};
    dataFull += "init,";

    //Guardar archivo en estring
    while(fgets(data, SIZE, fp) != NULL){

        string s(data);
        dataFull += s;
        cout << "(" << data << ")\n";
        cout << " * " << dataFull << "*\n";  
    }

    int senRes = send(sockfd, dataFull.c_str(), dataFull.size() + 1, 0);

    //bzero(dataFull, SIZE);


    printf("[+]Archivo enviado.\n");

    printf("[+]Cerrando conexion.\n");

    return 0;

}