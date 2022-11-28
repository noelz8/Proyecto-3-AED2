#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <string.h>


using namespace std;

int main(){
    string file1,file2;
    ifstream Compa,Compa2;

    file1="Compa.txt";
    file2="Compa2.txt";

    Compa.open( file1.c_str(),ios::binary);
    Compa2.open( file2.c_str(),ios::binary);

    if(!Compa){
        cout<< "No se pudo abrir el archivo"<<file1<<endl;
        return 1;
    }

    if(!Compa2){
        cout<< "No se pudo abrir el archivo"<<file2<<endl;
        return 1;
    }

    //comparar numero de lineas//
    
    int c1,c2;
    c1=0;
    c2=0;
    string str;
    while(!Compa.eof()){
        getline(Compa,str);
        c1++;
    }
     while(!Compa2.eof()){
        getline(Compa,str);
        c2++;
    }

    Compa.clear();
    Compa.seekg(0,ios::beg);

    Compa2.clear();
    Compa2.seekg(0,ios::beg);

    if(c1 != c2){
        cout<< "Diferencua de numero de lineas en archivos" << "\n";
        cout<< file1<<  "tine"<<"c1" "lineas y" << file2 <<"tine" <<"lineas" << "\n";
        return 1;

    }

    char string1[256], string2[256];
    int j=0, error_count=0;
    while(!Compa.eof()){
        Compa.getline(string1,256);
        Compa2.getline(string2,256);
        j++;
        if(strcmp(string1,string2)!=0){
            cout<<j<< "strings no iguales" <<endl;
            cout<< "file1" <<string1<<endl;
            cout<< "file2" <<string2<<endl;
            error_count++;
        }

    }
    if (error_count>0){
        cout<<"Los archivos son diferentes"<<endl;
    }
    else{
        cout<<"los archivos son iguales"<<endl;
    }
    return 0;



}