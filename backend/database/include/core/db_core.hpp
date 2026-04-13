/***
 * Database core module (header).
 ***/

#ifndef DB_CORE_HPP_
#define DB_CORE_HPP_

#include <exception>
#include <iostream>
#include <memory>
#include <pqxx/pqxx>
#include <utility>
#include "db_config.hpp"

// project namespace, database module namespace
namespace roomsched::db {
class database {
private:
    std::unique_ptr<pqxx::connection> conn;
    db_config config;  // TODO: later log from special config file - ?
    bool is_connected = false;

public:
    database(const db_config &config_);

    bool connect();
    void disconnect();
    bool is_connected_db() const;

    template <typename... Args>
    pqxx::result query(const std::string &sql, Args &&...args) {
        if (!is_connected || !conn) {
            throw std::runtime_error("Database not connected");
        }

        try {
            pqxx::work txn(*conn);  // transaction
            pqxx::result result =
                txn.exec_params(sql, std::forward<Args>(args)...);
            txn.commit();

            return result;
        } catch (const pqxx::sql_error &e) {
            std::cerr << "SQL error: " << e.what() << "\nQuery: " << e.query()
                      << std::endl;
            throw;
        } catch (const std::exception &e) {
            std::cerr << "Database exception: " << e.what() << std::endl;
            throw;
        }
    }

    template <typename... Args>
    void execute(const std::string &sql, Args &&...args) {
        if (!is_connected || !conn) {
            throw std::runtime_error("Database not connected");
        }

        try {
            pqxx::work txn(*conn);  // transaction
            txn.exec_params(sql, std::forward<Args>(args)...);

            txn.commit();
        } catch (const pqxx::sql_error &e) {
            std::cerr << "SQL error: " << e.what() << "\nQuery: " << e.query()
                      << std::endl;
            throw;
        } catch (const std::exception &e) {
            std::cerr << "Database exception: " << e.what() << std::endl;
            throw;
        }
    }
};
}  // namespace roomsched::db

#endif  // DB_CORE_HPP_
