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

#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <chrono>

#include <cmath>

using namespace std;
#define SIZE 1024
#define puerto 8080

#define SERVER "localhost"
#define USER "root"
#define PASSWORD "123"
#define DATABASE "db_pruebas"

#define ll unsigned long long int

/**
* @brief Funciones del Servidor de aquí en adelante 
* 
* 
*/
/**
 * @brief Huffman compression namespace
 */
namespace Huffman {

std::string HuffmanValue[256] = {""};

/// @brief structure for storing nodes.
struct Node {
    char character;
    ll count;
    Node *left, *right;

    Node(ll count) {
        this->character = 0;
        this->count = count;
        this->left = this->right = nullptr;
    }

    Node(char character, ll count) {
        this->character = character;
        this->count = count;
        this->left = this->right = nullptr;
    }
};
/**
 * @brief Common function necessary for both compression and decompression.
 */
namespace Utility {
    /**
     * @brief Get size of the file
     * @param filename name of the file.
     * @returns the filesize
     */
    ll get_file_size(const char *filename) {
        FILE *p_file = fopen(filename, "rb");
        fseek(p_file, 0, SEEK_END);
        ll size = ftello64(p_file);
        fclose(p_file);
        return size;
    }
    /**
     * @brief Test function to print huffman codes for each character. 
     */
    void Inorder(Node *root, std::string &value) {
        if (root) {
            value.push_back('0');
            Inorder(root->left, value);
            value.pop_back();

            if (root->left == nullptr && root->right == nullptr) {
                printf("Character: %c, Count: %lld, ", root->character, root->count);
                std::cout << "Huffman Value: " << value << std::endl;
            }
            
            value.push_back('1');
            Inorder(root->right, value);
            value.pop_back();
        }
    }
};

/**
 * @brief Functions necessary for compression.
 */
namespace CompressUtility {

/**
 * @brief Combine two nodes
 * @param a first node
 * @param b second node
 * @returns a node with a left and b right child.
 */
Node *combine(Node *a, Node *b) {
    Node *parent = new Node((a ? a->count : 0)+(b ? b->count : 0));
    parent->left = b;
    parent->right = a;
    return parent;
}

/**
 * @brief comparison function.
 * @param a first node
 * @param b second node
 * @returns true if first node is greater
 */
bool sortbysec(const Node *a, const Node *b) { 
    return (a->count > b->count); 
}

/**
 * @details Parses the file for character count
 * @param filename name of the file.
 * @param Filesize size of the file.
 * @returns count of all present characters in file as a map
*/
std::map<char, ll>parse_file(const char* filename, const ll Filesize) {
    FILE *ptr = fopen(filename, "rb");
    if (ptr == NULL) {
        perror("Error: File not found:");
        exit(-1);
    }
    unsigned char ch;
    ll size = 0, filesize = Filesize;
    std::vector<ll>Store(256, 0);

    while (size != filesize) {
        ch = fgetc(ptr);
        ++Store[ch];
        ++size;
    }

    std::map<char, ll>store;
    for (int i = 0; i < 256; ++i) {
        if (Store[i]) {
            store[i] = Store[i];
        }
    }
    fclose(ptr);
    return store;
}
/**
 * @details Utility function to sort array by character count
 */
std::vector<Node*>sort_by_character_count(const std::map<char, ll>&value) {
    std::vector<Node*> store;
    for (auto &x: value) {
        store.push_back(new Node(x.first, x.second));
    }
    sort(store.begin(), store.end(), sortbysec);
    return store;
}
/**
 * @brief Generate a header for the file.
 * Format: 
 * 1. Total Unique Character (1 byte)
 * 2. For each unique character:
 * 2a. Character (1 byte)
 * 2b. Length of code (1 byte)
 * 2c. Huffman code (min: 1 byte, max: 255bytes)
 * 3. Padding
 * Worst case header size: 1 + (1+1)*(1+2+3+4+5+...+255) + 1 ~ 32kb... (only happens when skewed Huffman tree is generated)
 * Best case header size: 1 + 1 + 1 + 1 + 1 = 5bytes (Happens only when a single character exists in an entire file).
 */
std::string generate_header(const char padding) {
    std::string header = "";
    // UniqueCharacter start from -1 {0 means 1, 1 means 2, to conserve memory}
    unsigned char UniqueCharacter = 255;
    
    for (int i = 0; i < 256; ++i) {
        if (HuffmanValue[i].size()) {
            header.push_back(i);
            header.push_back(HuffmanValue[i].size());
            header += HuffmanValue[i];
            ++UniqueCharacter;
        }
    }
    char value = UniqueCharacter;
    return value+header+(char)padding;
}

/**
 * @details Store Huffman values for each character in string. 
 * @param root root of the huffman tree
 * @param value binary string
 * @returns the size of the resulting file (without the header)
 */
ll store_huffman_value(const Node *root, std::string &value) {
    ll temp = 0;  
    if (root) {
        value.push_back('0');
        temp = store_huffman_value(root->left, value);
        value.pop_back();
        if (!root->left && !root->right) {
            HuffmanValue[(unsigned char)root->character] = value;
            temp += value.size() * root->count;
        }
        value.push_back('1');
        temp += store_huffman_value(root->right, value);
        value.pop_back();
    }
    return temp;
}

/**
 * @details Create huffman tree during compression...
 * @param value mapping of character counts.
 * @returns root of the huffman tree.
 */
Node *generate_huffman_tree(const std::map <char, ll>&value) {
    std::vector<Node*> store = sort_by_character_count(value);
    Node *one, *two, *parent;
    sort(begin(store), end(store), sortbysec);
    if (store.size() == 1) {
        return combine(store.back(), nullptr);
    }
    while (store.size() > 2) {
        one = *(store.end() - 1); two = *(store.end() - 2);
        parent = combine(one, two);
        store.pop_back(); store.pop_back();
        store.push_back(parent);

        std::vector<Node*>::iterator it1 = store.end() - 2;
        while ((*it1)->count < parent->count && it1 != begin(store)) {
            --it1;
        }
        std::sort(it1, store.end(), sortbysec);
    }
    one = *(store.end() - 1); two = *(store.end() - 2);
    return combine(one, two);
}
/**
 * @brief Actual compression of a file.
 * @param filename file to be compressed.
 * @param Filesize size of the file.
 * @param PredictedFileSize the size of the compressed file.
 * @returns void, but compresses the file as ${filename}.abiz
 */
void compress (const char *filename, const ll Filesize, const ll PredictedFileSize) {
    const char padding = (8 - ((PredictedFileSize)&(7)))&(7);
    const std::string header = generate_header(padding);
    int header_i = 0;
    const int h_length = header.size();
    std::cout << "Padding size: " << (int)padding << std::endl;
    FILE *iptr = fopen(filename, "rb"), *optr = fopen((std::string(filename)+".abiz").c_str(), "wb");
    
    if (!iptr) {
        perror("Error: File not found: ");
        exit(-1);
    }

    while (header_i < h_length) {
        fputc(header[header_i], optr);
        ++header_i;
    }

    unsigned char ch, fch = 0;
    char counter = 7;
    ll size = 0, i;
    while(size != Filesize) {
        ch = fgetc(iptr);
        i = 0;
        const std::string &huffmanStr = HuffmanValue[ch];
        while(huffmanStr[i] != '\0') {
            fch = fch | ((huffmanStr[i] - '0') << counter);
            // Decrement from 7 down to zero, and then
            // back again at 7
            counter = (counter + 7) & 7;
            if(counter == 7) {
                fputc(fch, optr);
                fch ^= fch;
            }
            ++i;
        }
        ++size;
        if((size * 100 / Filesize) > ((size - 1) * 100 / Filesize)) {
            printf("\r%lld%% completed  ", (size * 100 / Filesize));
        }
    }
    if(fch) {
        fputc(fch, optr);
    }
    printf("\n");
    fclose(iptr);
    fclose(optr);
}

};
/**
 * @brief Functions necessary for decompression.
 */
namespace DecompressUtility {
/**
 * @details Generate huffman tree based on header content
 */
void generate_huffman_tree(Node * const root, const std::string &codes, const unsigned char ch) {
    Node *traverse = root;
    int i = 0;
    while(codes[i] != '\0') {
        if(codes[i] == '0') {
            if(!traverse->left) {
                traverse->left = new Node(0);
            }
            traverse = traverse->left;
        } else {
            if(!traverse->right) {
                traverse->right = new Node(0);
            }
            traverse = traverse->right;
        }
        ++i;
    }
    traverse->character = ch;
}
/**
 * @brief Function to store and generate a tree
 * @param iptr file pointer
 * @returns root of the node and pair of values,
 * first containing padding to complete a byte and 
 * total_size
 */
std::pair<Node*, std::pair<unsigned char, int>>decode_header(FILE *iptr) {
    Node *root = new Node(0);
    int charactercount, buffer, total_length = 1;
    char ch, len;
    charactercount = fgetc(iptr);
    std::string codes;
    ++charactercount;
    while(charactercount) {
        ch = fgetc(iptr);
        codes = ""; 
        len = fgetc(iptr);
        buffer = len;

        while(buffer > codes.size()) {
            codes.push_back(fgetc(iptr));
        }
        // character (1byte) + length(1byte) + huffmancode(n bytes where n is length of huffmancode)
        total_length += codes.size()+2;

        generate_huffman_tree(root, codes, ch);
        --charactercount;
    }
    unsigned char padding = fgetc(iptr);
    ++total_length;
    return {root, {padding, total_length}};
}
/**
 * @details Decompresses the given .abiz file.
 * @param filename name of the file
 * @param Filesize file size
 * @param leftover 
 * @returns void, but decompresses the file and stores it as
 * output${filename} (without the .abiz part)
 */
void decompress(const char*filename, const ll Filesize, const ll leftover) {
    const std::string fl = filename;
    FILE *iptr = fopen(fl.c_str(), "rb");
    FILE *optr = fopen(std::string("output"+fl.substr(0, fl.size() - 5)).c_str(), "wb");
    
    if (iptr == NULL) {
        perror("Error: File not found");
        exit(-1);
    }

    std::pair<Node*, std::pair<unsigned char, int>>HeaderMetadata = decode_header(iptr);
    Node *const root = HeaderMetadata.first;
    const auto [padding, headersize] = HeaderMetadata.second;

    char ch, counter = 7;
    ll size = 0;
    const ll filesize = Filesize-headersize;
    Node *traverse = root;
    ch = fgetc(iptr);
    while (size != filesize) {
        while (counter >= 0) {
            traverse = ch & (1 << counter) ? traverse->right : traverse->left;
            ch ^= (1 << counter);
            --counter;
            if (!traverse->left && !traverse->right) {
                fputc(traverse->character, optr);
                if(size == filesize - 1 && padding == counter + 1) {
                    break;
                }
                traverse = root;
            }
        }
        ++size;
        counter = 7;
        if ((size * 100 / filesize) > ((size - 1) * 100 / filesize)) {
            printf("\r%lld%% completed, size: %lld bytes   ", (size * 100 / filesize), size);
        }
        ch = fgetc(iptr);
    }
    fclose(iptr);
    fclose(optr);
}

} /// namespace DecompressUtility

}; /// namespace Huffman

using namespace Huffman;

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
    char buf3[SIZE];
    char fact[4096];


    const char *filename = "Pruebas.txt", *filename2 = "Pruebas.txt.abiz";
    printf("%s\n", filename);
    string comandostring = "";
    string ParametroString = "";
    string comando3 = " ";

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
        memset(buf3, 0, SIZE);
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
        ParametroString = buf2;
         // Esperando a que el cliente envie el mensaje
        /*int bytesReceived3 = recv(clientSocket, buf3, 4096, 0);
        if (bytesReceived3 == -1)
        {
            cerr << "Error in recv(). Quitting" << endl;
            break;
        }
 
        if (bytesReceived3 == 0)
        {
            cout << "Client disconnected " << endl;
            break;
        }
        comando3 = buf3;
        
        char *mensaje3 = "Comando aceptado exitosamente";*/

        //Le pasamos el comando char a un string global
        

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

        if (comandostring == "init"){
            cout << "\n Se crea nuevo repopositorio \n";
            
            string linescomando = "Create table " + ParametroString + " (ID INT NOT NULL AUTO_INCREMENT, NOMBRE_ARCHIVO VARCHAR(50) NOT NULL, PRIMARY KEY (ID));";
            mysql_query (connect, linescomando.c_str());
            unsigned int i =0;

            mysql_close (connect);
            //Envio de mensaje al cliente s
            char *mensaje2 = "Se ha creado nuevo repositorio ";
            send(clientSocket, mensaje2, strlen(mensaje2) + 1, 0); 
        }

        //Prueba add
        if (comandostring == "add"){
            cout << "\n Se crea nuevo repopositorio \n";
            
            string linescomandoAdd = "INSERT INTO" + ParametroString + "(NOMBRE_ARCHIVO) values ('Nuevoarch.txt');";
            mysql_query (connect,linescomandoAdd.c_str());
            //mysql_query (connect, linescomando.c_str());
            unsigned int i =0;

            mysql_close (connect);
            //Envio de mensaje al cliente s
            char *mensaje2 = "hay datos que necesitan commit... ";
            send(clientSocket, mensaje2, strlen(mensaje2) + 1, 0); 
        }
    }

    //Compresion Huffman
    std::chrono::time_point <std::chrono::system_clock> start, end; 
    std::chrono::duration <double> time;
    ll filesize, predfilesize;

    filesize = Utility::get_file_size(filename);
    auto mapper = CompressUtility::parse_file(filename, filesize);
    Node *const root = CompressUtility::generate_huffman_tree(mapper);
    std::string bufe = "";
    predfilesize = CompressUtility::store_huffman_value(root, bufe);
    printf("Original File: %lld bytes\n", filesize);
    printf("Compressed File Size (without header): %lld bytes\n", (predfilesize+7)>>3);

    start = std::chrono::system_clock::now();
    CompressUtility::compress(filename, filesize, predfilesize);
    end = std::chrono::system_clock::now();

    time = (end - start);
    std::cout << "Compression Time: " << time.count() << "s" << std::endl;


    filesize = Utility::get_file_size(filename2);
    start = std::chrono::system_clock::now();
    DecompressUtility::decompress(filename2, filesize, predfilesize);
    end = std::chrono::system_clock::now();

    time = (end - start);
    std::cout << "\nDecompression Time: " << time.count() << "s" << std::endl;

    //Finaliza la conexión
    close(clientSocket);
    return 0;
}