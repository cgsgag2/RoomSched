/***
 * Database core module (source).
 ***/

#include <iostream>
#include <memory>
#include <pqxx/pqxx>
#include <string>
// Local files
#include "core/db_core.hpp"

roomsched::db::database::database(const db_config &config_) : config(config_) {
}

bool roomsched::db::database::connect() {
    std::cout << "----------Init data base----------" << std::endl;

    // Connect to DB
    try {
        const std::string conn_str =
            "host=" + config.DB_HOST + " dbname=" + config.DB_NAME +
            " user=" + config.DB_USER + " password=" + config.DB_PASSWORD;

        conn = std::make_unique<pqxx::connection>(conn_str);

        if (conn->is_open()) {
            std::cout << "Connected to DB '" << config.DB_NAME
                      << "' succcessfully!" << std::endl;
            is_connected = true;
            return true;
        } else {
            std::cout << "Connection FAILED!" << std::endl;
            is_connected = false;
            return false;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error in connection to DB: " << e.what() << std::endl;
        is_connected = false;
        return false;
    }
}

void roomsched::db::database::disconnect() {
    if (conn && conn->is_open()) {
        try {
            conn->disconnect();
            std::cout << "Disconnected from db '" << config.DB_NAME
                      << "' successfully" << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Error while disconnecting from DB: " << e.what()
                      << std::endl;
        }
    }

    conn.reset();
    is_connected = false;
}

bool roomsched::db::database::is_connected_db() const {
    return is_connected;
}
