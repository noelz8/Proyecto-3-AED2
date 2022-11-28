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
#define puerto 8080
using namespace std;

string dataFull;

string filename1;


int main(int argc, char *argv[]){   
    //Variables necesarias para el ingreso de comandos
    const char *comando1 = argv[1],
               *comando2 = argv[2],
               *comando3 = argv[3],
               *comando4 = argv[4];
    char *ip = "127.0.0.1";
    //int puerto = 8080;
    int coneccion;

    int sockfd;
    FILE *fp;
    char filename[SIZE];
    char palab[SIZE];
    string palabra; 

    //Validamos cual sera el número de argumentos
    if (argc == 3){

        //Validamos que se inicie con el comando got
        if (string(comando1) != "got"){

            cout << "Error, no uso got para ejecutar los comandos";
            cout << "!!!..O no ha ingresado comando valido..!!!";

        }else if (string(comando2) != "help"){

            cout << " El segundo argumento no es valido \n";
            cout << " Verifique escribir bien ** got help **";
            exit(1);

        }else{
            //dddd
        }

    }else if (argc == 4)
    {   
        //Validamos que se inicie con el comando got
        if (string(comando1) != "got"){

            cout << "Error, no uso got para ejecutar los comandos";
            cout << "!!!..O no ha ingresado comando valido..!!!";

        }else{

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
            dataFull += string(comando2);
            dataFull += ",";

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

        }
        
    }else if (argc == 5)
    {
        /* code */
    }else{
        cout << "Error al ingresar el comando...\n";
        cout << " consultar comando  ** got help **\n";
        exit(1);
    }

    return 0;
}