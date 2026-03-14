/***
 * Database user repository module (source).
 ***/

#include "repositories/user_repository.hpp"
#include <iostream>

roomsched::db::user_repository::user_repository(database &db_) : db(db_) {
}

bool roomsched::db::user_repository::create_user(const user &new_user) {
    try {
        db.execute(
            "INSERT INTO users (username, password_hash) VALUES ($1, $2)",
            new_user.username, new_user.password
        );
        return true;
    } catch (const pqxx::unique_violation &) {
        std::cerr << "[NOTE]: user '" << new_user.username << "' already exists"
                  << std::endl;
        return false;
    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in create_user]: " << e.what() << std::endl
                  << "Query: " << e.query() << std::endl;
        return false;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in creating user]: " << e.what()
                  << std::endl;
        return false;
    }
}

roomsched::db::user roomsched::db::user_repository::get_user_by_id(int id) {
    // TODO!
    user a{};
    return a;
}

std::vector<roomsched::db::user> roomsched::db::user_repository::get_all_users(
) {
    std::vector<user> all_users;

    try {
        const auto result = db.query(
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
            all_users.push_back(current_user);
        }
    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in get_all_users]: " << e.what() << std::endl
                  << "Query: " << e.query() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in get_all_users]: " << e.what()
                  << std::endl;
    }
    return all_users;
}

void roomsched::db::user_repository::print_all_users() {
    // TODO: it is test function, don't really need
    // Maybe do it for tests
}

bool roomsched::db::user_repository::entry_user(
    const std::string &username,
    const std::string &password
) {
    try {
        // TODO: real hash
        const std::string &password_hash = password;

        const auto result = db.query(
            "SELECT id FROM users WHERE username = $1 AND password_hash = $2",
            username, password_hash
        );

        if (!result.empty()) {
            db.execute(
                "UPDATE users SET last_login = NOW() WHERE username = $1",
                username
            );
            return true;
        }

        return false;

    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in entry_user]: " << e.what() << std::endl
                  << "Query: " << e.query() << std::endl;
        return false;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in entry_user]: " << e.what() << std::endl;
        return false;
    }
}
