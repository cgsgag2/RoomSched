/* First test to DB */
/*
#ifndef DB_MAIN_HPP_
#define DB_MAIN_HPP_

#include <iostream>
#include <pqxx/pqxx>
#include <vector>

namespace roomsched {
class data_base_info {
public:
    std::string text;
    int number;

    void print() const {
        std::cout << "TEXT from DB: " << text << "\nNumber from DB: " << number
                  << std::endl;
    }
};

class database_main {
private:
    std::string connectionString;

public:
    database_main(
        const std::string &host = "localhost",
        const std::string &port = "5432",
        const std::string &dbname = "first_test_db",
        const std::string &user = "postgres",
        const std::string &password = "password"
    ) {
        connectionString = "host=" + host + " " + "port=" + port + " " +
                           "dbname=" + dbname + " " + "user=" + user + " " +
                           "password=" + password;
    }

    void try_connection() {
        try {
            pqxx::connection conn(connectionString);
            if (conn.is_open()) {
                std::cout << "Connected to data base " << conn.dbname()
                          << " successfully" << std::endl;
            } else {
                std::cout << "Connection FAILED!" << std::endl;
            }
        } catch (const std::exception &e) {
            std::cerr << "Error in connecting: " << e.what() << std::endl;
        }
    }

    std::vector<data_base_info> getInfoFromDB() {
        std::vector<data_base_info> all_info;

        try {
            pqxx::connection conn(connectionString);
            pqxx::work txn(conn);

            std::string query = "SELECT * FROM first_info";
            pqxx::result result = txn.exec(query);

            for (const auto &row : result) {
                data_base_info inf;
                inf.text = row["message"].as<std::string>();
                inf.number = row["num"].as<int>();
                all_info.push_back(inf);
            }

            txn.commit();
        } catch (const std::exception &e) {
            std::cerr << "Error in SELECT query: " << e.what() << std::endl;
        }
        return all_info;
    }
};

}  // namespace roomsched

#endif  // DB_MAIN_HPP_
*/
