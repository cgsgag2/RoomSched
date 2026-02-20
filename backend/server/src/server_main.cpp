/***
 * Server (test) module
 ***/

#include <postgresql/libpq-fe.h>
#include <iostream>

int main() {
    PGconn *conn = PQconnectdb("dbname=first_test_db user=postgres");
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << PQerrorMessage(conn);
        PQfinish(conn);
        return 0;
    }

    PGresult *res = PQexec(conn, "SELECT message FROM test");

    std::cout << PQgetvalue(res, 0, 0) << std::endl;

    PQclear(res);
    PQfinish(conn);
    return 0;
}
