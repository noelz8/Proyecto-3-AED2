//Instalar esta librería par apoder usar mysql
//sudo apt-get install libmsqlclient-dev
//sudo apt-get install libmysql++-dev
//sudo apt-get install libmysql++3 --> esta no es tan necesaria

#include <iostream>
#include <stdlib.h>
#include <mysql/mysql.h>

using namespace std;

struct connection_details{
    const char *server, *user, *password, *database;
};

//Esta configuración toma la conexión a la base de Mysql
MYSQL* mysql_connection_setup(struct connection_details mysql_details){
    MYSQL *connection = mysql_init(NULL);

    //Caso de error
    if (!mysql_real_connect(connection, mysql_details.server, mysql_details.user, mysql_details.password,
    mysql_details.database, 0, NULL, 0)){
        cout << "Error en la conexión a la base de datos: " << mysql_error(connection);
        exit(1);
    }
    return connection;
}

MYSQL_RES* mysql_execute_query(MYSQL *connection, const char *sql_query){
    //Cado de error
    if (mysql_query(connection, sql_query)){
        cout << "Error en la consulta: " << mysql_error(connection);
        exit(1);
    }
    return mysql_use_result(connection);
}

int main(){
    
    MYSQL *conx;
    MYSQL_RES *res;
    MYSQL_ROW  row;

    //Referenciamos la conexion
    struct connection_details _contexMysql;
    _contexMysql.server = "localhost";
    _contexMysql.user = "root";
    _contexMysql.password = "123";
    _contexMysql.database = "db_pruebas";

    conx = mysql_connection_setup(_contexMysql);
    res = mysql_execute_query(conx, "SELECT * from tb_datos;");
    //cout << res;

    mysql_free_result(res);
    mysql_close(conx);

    return 0;
}



