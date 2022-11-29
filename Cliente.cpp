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

string dataComando2;
string dataComando3;
string filename1;

int main(int argc, char *argv[]){   
    //Variables necesarias para el ingreso de comandos
    const char *comando1 = argv[1],
               *comando2 = argv[2],
               *comando3 = argv[3],
               *comando4 = argv[4];

    char buf[SIZE];

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
            cout << " Verifique escribir bien * got help *";
            exit(1);

        }else{

            //Se conecta o muestra error en dado caso
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0){
                perror("[-]Error en socket");
                exit(1);
            }

            printf("[+]Socket creado correctamente \n");

            //importante para la coneción al servidor y cliente
            sockaddr_in hint;
            hint.sin_family = AF_INET;
            hint.sin_port = htons(puerto);
            inet_pton(AF_INET, ip, &hint.sin_addr);

            //Coneción por socket
            coneccion = connect(sockfd, (struct sockaddr*)&hint, sizeof(hint));
            if(coneccion == -1){
                perror("[-]Error en el socket");
                exit(1);
            }
            printf("[+]Conectando al servidor\n");


            int n;
            char data[SIZE] = {0};

            //Se guarda el comando y el mensaje 
            dataComando2 += string(comando2);

            //Se envia mensaje al servidor con el comando
            int senRes = send(sockfd, dataComando2.c_str(), dataComando2.size() + 1, 0);

            printf("[+]Comendo enviado.\n");

            memset(buf, 0, SIZE);

            //Validación en caso de tener algún problema
            int bytesReceived2 = recv(sockfd, buf, 4096, 0);
            if (bytesReceived2 == -1)
            {
                cout << "Ocurrio un error obteniendo la respuesta del servidor\r\n";
            }

            //Respuesta desde el servidor
            cout << "SERVER> " << string(buf, bytesReceived2) << "\r\n";

            cout << "-------------------------------------------------------------------------------------------\n";
            cout << "*got help: Muestra los comandos y sus funciones.                                             \n";
            cout << "-------------------------------------------------------------------------------------------\n";
            cout << "*got init <name>: Instancia un nuevo repositio.                                              \n";
            cout << "-------------------------------------------------------------------------------------------\n";
            cout << "*got add [-A] / [path]: [-A] para agregar todos los archivos, o la ruta del archivo para \n";
            cout << " guardar uno en especifico.\n";
            cout << "-------------------------------------------------------------------------------------------\n";
            cout << "*got commit <mensaje> : Envia los archivos agregados y pendientes.                         \n";
            cout << " El tercer argumento debe ser un string(" ").\n";
            cout << "-------------------------------------------------------------------------------------------\n";
            cout << "*got status : Nos muestra cuales archivos han sido cambiados, agregados o eliminados.        \n";
            cout << "-------------------------------------------------------------------------------------------\n";
            cout << "*got rollback <file> <commit> : Permite regresar un archivo en el tiempo a un commit\n";
            cout << " especifico.El tercer y cuarto argumento debe ser un string(" ").\n";
            cout << "-------------------------------------------------------------------------------------------\n";
            cout << "*got reset <file> : Deshace cambios locales para un archivo y lo regresa al ultimo commit    \n";
            cout << " El tercer argumento debe ser un string(" ").\n";
            cout << "-------------------------------------------------------------------------------------------\n";
            cout << "*got sync <file> : Recupera los cambios para un archivo en el server y lo sincroniza\n";
            cout << " con el archivo en el cliente. El tercer argumento debe ser un string(" ").\n";
            cout << "-------------------------------------------------------------------------------------------\n";          
            printf("[+]Cerrando conexion.\n");
        }

    }else if (argc == 4)
    {   
        //Validamos que se inicie con el comando got
        if (string(comando1) != "got"){

            cout << "Error, no uso got para ejecutar los comandos";
            cout << "!!!..O no ha ingresado comando valido..!!!";

        }else{

            if ((string(comando2) == "init") || (string(comando2) == "add") 
               || (string(comando2) == "commit") || (string(comando2) == "status") 
               ||(string(comando2) == "reset") || (string(comando2) == "sync")){
                
                //Se conecta o muestra error en dado caso
                sockfd = socket(AF_INET, SOCK_STREAM, 0);
                if (sockfd < 0){
                    perror("[-]Error en socket");
                    exit(1);
                }

                printf("[+]Socket creado correctamente \n");

                //importante para la coneción al servidor y cliente
                sockaddr_in hint;
                hint.sin_family = AF_INET;
                hint.sin_port = htons(puerto);
                inet_pton(AF_INET, ip, &hint.sin_addr);

                //Coneción por socket
                coneccion = connect(sockfd, (struct sockaddr*)&hint, sizeof(hint));
                if(coneccion == -1){
                    perror("[-]Error en el socket");
                    exit(1);
                }
                printf("[+]Conectando al servidor\n");

                int n;
                char data[SIZE] = {0};

                //Se guarda el comando y el mensaje 
                dataComando2 += string(comando2);
                dataComando3 += string(comando3);

                //Se envia mensaje al servidor con el comando
                int senRes = send(sockfd, dataComando2.c_str(), dataComando2.size() + 1, 0);

                printf("[+]Comendo enviado.\n");

                memset(buf, 0, SIZE);

                //Validación en caso de tener algún problema
                int bytesReceived2 = recv(sockfd, buf, 4096, 0);
                if (bytesReceived2 == -1)
                {
                    cout << "Ocurrio un error obteniendo la respuesta del servidor\r\n";
                }

                //Respuesta desde el servidor
                cout << "SERVER> " << string(buf, bytesReceived2) << "\r\n";

                senRes = send(sockfd, dataComando3.c_str(), dataComando3.size() + 1, 0);
                
                printf("[+]Parametro enviado enviado.\n");

                memset(buf, 0, SIZE);

                //Validación en caso de tener algún problema
                int bytesReceived = recv(sockfd, buf, 4096, 0);
                if (bytesReceived == -1)
                {
                    cout << "Ocurrio un error obteniendo la respuesta del servidor\r\n";
                }

                //Respuesta desde el servidor
                cout << "SERVER> " << string(buf, bytesReceived) << "\r\n";

                printf("[+]Cerrando conexion.\n");

            
            }else{
                cout << "El comando no es valido \n";
                cout << " consultar comando  * got help *\n";
                exit(1);
            }
        }
        
    }else if (argc == 5)
    {
        //Validamos que se inicie con el comando got
        if (string(comando1) != "got"){

            cout << "Error, no uso got para ejecutar los comandos";
            cout << "!!!..O no ha ingresado comando valido..!!!";

        }else{

            if (string(comando2) == "rollback"){
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


                int n;
                char data[SIZE] = {0};
                
                /*dataFull += string(comando2);
                dataFull += ",";
                dataFull += string(comando3);
                dataFull += ",";
                dataFull += string(comando4);

                int senRes = send(sockfd, dataFull.c_str(), dataFull.size() + 1, 0);
                */
                printf("[+]Archivo enviado.\n");

                printf("[+]Cerrando conexion.\n");

            
            }else{
                cout << "El comando no es valido \n";
                cout << " consultar comando  * got help *\n";
                exit(1);
            }
        }    
    }else{
        cout << "Error al ingresar el comando...\n";
        cout << " consultar comando  * got help *\n";
        exit(1);
    }
    return 0;
}

/*
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
*/