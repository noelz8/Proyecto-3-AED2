#include <iostream>
#include <fstream>
using namespace std;



int main()
{
    ifstream archivo("Pruebas.txt");
    char linea[128];
    long contador = 0L;

    if(archivo.fail())
    cerr << "Error al abrir el archivo Pruebas.txt" << endl;
    else
    while(!archivo.eof())
    {
        archivo.getline(linea, sizeof(linea));
        cout << linea << endl;
        if((++contador % 24)==0)
        {
        cout << "CONTINUA...";
        cin.get();
    }
    }
    archivo.close();
   return 0;
}
