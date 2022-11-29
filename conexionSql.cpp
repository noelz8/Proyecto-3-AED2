//Instalar esta librería par apoder usar mysql
//sudo apt-get install libmsqlclient-dev
//sudo apt-get install libmysql++-dev
//sudo apt-get install libmysql++3 --> esta no es tan necesaria

//La linea para poder correrlo es por este

//  g++ -o main conexionSql.cpp  -L/usr/include/mysql -lmysqlclient  -I/usr/include/mysql


#include<iostream>
#include<mysql/mysql.h>
#include<stdio.h>

using namespace std;

#define SERVER "localhost"
#define USER "root"
#define PASSWORD "123"
#define DATABASE "db_pruebas"

int main()
{

MYSQL *connect;
connect=mysql_init(NULL);

if (!connect)
{
cout<<"MySQL Initialization failed";
return 1;
}

connect=mysql_real_connect(connect, "localhost", "root", "123" , "db_pruebas" ,0,NULL,0);

if (connect)
{
cout<<"connection Succeeded\n";
}
else
{
cout<<"connection failed\n";
}

MYSQL_RES *res_set;
MYSQL_ROW row;

mysql_query (connect,"INSERT INTO tb_datos (ID, Nombre) VALUES ('3', 'Jose'), ('4', 'Monica');");
mysql_query (connect,"select * from tb_datos;");
unsigned int i =0;
res_set = mysql_store_result(connect);
unsigned int numrows = mysql_num_rows(res_set);

cout << endl;
cout <<"\t ---------------------------------------------------------------- \t"<< endl;

while (((row= mysql_fetch_row(res_set)) !=NULL ))
{ //cout<<" %s\n",row[i] !=NULL?row[i] : "NULL";

cout <<"\t | \t" << row[i] << "\t | \t"<<  row[i+1] << "\t | \t"<< row[i+2] << "\t | \t" << endl;
cout <<"\t ---------------------------------------------------------------- \t"<< endl;
}

mysql_close (connect);
return 0;
}