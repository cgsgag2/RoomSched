/***
 * Database manager module (source).
 ***/

#include "db_manager.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <pqxx/pqxx>
#include <string>
#include "db_users.hpp"

bool roomsched::db::database_manager::init() {
    std::cout << "----------Init data base----------" << std::endl;

    // Connect to DB
    try {
        const std::string conn_str = "host=" + DB_HOST + " dbname=" + DB_NAME +
                                     " user=" + DB_USER +
                                     " password=" + DB_PASSWORD;

        conn = std::make_unique<pqxx::connection>(conn_str);

        if (conn->is_open()) {
            std::cout << "Connected to DB '" << DB_NAME << "' succcessfully!"
                      << std::endl;
            return true;
        } else {
            std::cout << "Connection FAILED!" << std::endl;
            return false;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error in connection to DB: " << e.what() << std::endl;
        return false;
    }

    return false;
}

bool roomsched::db::database_manager::create_user(
    const std::string &username,
    const std::string &password
) {
    try {
        pqxx::work txn(*conn);

        // TODO: real hash (to passwords)
        const std::string &password_hash = password;

        txn.exec_params(
            "INSERT INTO users (username, password_hash) VALUES ($1, $2)",
            username, password_hash
        );

        txn.commit();
        std::cout << "User " << username << " was created!" << std::endl;
        return true;
    } catch (const pqxx::unique_violation &) {
        std::cerr << "NOTE: user '" << username << "' already exists"
                  << std::endl;
        return false;
    } catch (const std::exception &e) {
        std::cerr << "Error in creating user: " << e.what() << std::endl;
        return false;
    }
}

void roomsched::db::database_manager::print_all_users() {
    try {
        pqxx::work txn(*conn);

        const pqxx::result result = txn.exec(
            "SELECT id, username, created_at, last_login FROM users ORDER BY id"
        );

        std::cout << "\n========== All users ==========\n";
        std::cout << "ID\tUsername\tCreated at\t\tLast login" << std::endl;
        std::cout << "---------------------------------------" << std::endl;

        for (const auto &row : result) {
            user current_user;
            current_user.id = row["id"].as<int>();
            current_user.username = row["username"].as<std::string>();
            current_user.created_at = row["created_at"].as<std::string>();
            current_user.last_login = row["last_login"].is_null()
                                          ? "Never"
                                          : row["last_login"].as<std::string>();
            std::cout << current_user;
        }
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Number of users: " << result.size() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error in query of users: " << e.what() << std::endl;
    }
}

std::vector<roomsched::db::user> roomsched::db::database_manager::get_all_users(
) {
    std::vector<user> all_users;

    try {
        pqxx::work txn(*conn);

        const pqxx::result result = txn.exec(
            "SELECT id, username, created_at, last_login FROM users ORDER BY id"
        );

        for (const auto &row : result) {
            user current_user;
            current_user.id = row["id"].as<int>();
            current_user.username = row["username"].as<std::string>();
            current_user.created_at = row["created_at"].as<std::string>();
            current_user.last_login = row["last_login"].is_null()
                                          ? "Never"
                                          : row["last_login"].as<std::string>();
            // std::cout << current_user;
            all_users.push_back(current_user);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error in query of users: " << e.what() << std::endl;
    }
    return all_users;
}

bool roomsched::db::database_manager::entry_user(
    const std::string &username,
    const std::string &password
) {
    try {
        pqxx::work txn(*conn);

        // TODO: real hash
        const std::string &password_hash = password;

        const pqxx::result result = txn.exec_params(
            "SELECT id FROM users WHERE username = $1 AND password_hash = $2",
            username, password_hash
        );

        if (!result.empty()) {
            txn.exec_params(
                "UPDATE users SET last_login = NOW() WHERE username = $1",
                username
            );
            txn.commit();
            return true;
        }

        return false;

    } catch (const std::exception &e) {
        std::cerr << "Error in authentication: " << e.what() << std::endl;
        return false;
    }
}
