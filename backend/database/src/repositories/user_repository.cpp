/***
 * Database user repository module (source).
 ***/

#include "repositories/user_repository.hpp"
#include <iostream>

namespace roomsched::db {

user_repository::user_repository(database &db_) : db(db_) {}

bool user_repository::create_user(const user &new_user) {
    try {
        db.execute(
            "INSERT INTO users (username, email, phone, password_hash, role) "
            "VALUES ($1, $2, $3, $4, $5)",
            new_user.username,
            new_user.email,
            new_user.phone,
            new_user.password_hash,
            new_user.role
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

bool user_repository::user_exists_by_email(const std::string &email) {
    try {
        auto result = db.query(
            "SELECT id FROM users WHERE email = $1",
            email
        );
        return !result.empty();

    } catch (const std::exception &e) {
        std::cerr << "[DB ERROR in user_exists_by_email]: " << e.what() << std::endl;
        return false;
    }
}

bool user_repository::user_exists_by_phone(const std::string &phone) {
    try {
        auto result = db.query(
            "SELECT id FROM users WHERE phone = $1",
            phone
        );
        return !result.empty();

    } catch (const std::exception &e) {
        std::cerr << "[DB ERROR in user_exists_by_phone]: " << e.what() << std::endl;
        return false;
    }
}

std::optional<user> user_repository::get_user_by_email(const std::string &email) {
    try {
        auto result = db.query(
            "SELECT id, username, email, phone, password_hash, role, created_at, last_login "
            "FROM users WHERE email = $1",
            email
        );

        if (result.empty()) {
            return std::nullopt;
        }

        const auto &row = result[0];
        user u;

        u.id = row["id"].as<int>();
        u.username = row["username"].as<std::string>();
        u.email = row["email"].as<std::string>();
        u.phone = row["phone"].as<std::string>();
        u.password_hash = row["password_hash"].as<std::string>();
        u.role = row["role"].as<std::string>();
        u.created_at = row["created_at"].as<std::string>();
        u.last_login = row["last_login"].is_null()
                           ? "Never"
                           : row["last_login"].as<std::string>();

        return u;

    } catch (const std::exception &e) {
        std::cerr << "[DB ERROR in get_user_by_email]: " << e.what() << std::endl;
        return std::nullopt;
    }
}

user user_repository::get_user_by_id(int id) {
    try {
        auto result = db.query(
            "SELECT id, username, email, phone, password_hash, role, created_at, last_login "
            "FROM users WHERE id = $1",
            id
        );

        if (result.empty()) {
            throw std::runtime_error("User not found");
        }

        const auto &row = result[0];
        user u;

        u.id = row["id"].as<int>();
        u.username = row["username"].as<std::string>();
        u.email = row["email"].as<std::string>();
        u.phone = row["phone"].as<std::string>();
        u.password_hash = row["password_hash"].as<std::string>();
        u.role = row["role"].as<std::string>();
        u.created_at = row["created_at"].as<std::string>();
        u.last_login = row["last_login"].is_null()
                           ? "Never"
                           : row["last_login"].as<std::string>();

        return u;

    } catch (const std::exception &e) {
        std::cerr << "[DB ERROR in get_user_by_id]: " << e.what() << std::endl;
        throw;
    }
}

std::vector<user> user_repository::get_all_users() {
    std::vector<user> users;

    try {
        auto result = db.query(
            "SELECT id, username, email, phone, password_hash, role, created_at, last_login "
            "FROM users"
        );

        for (const auto &row : result) {
            user u;
            u.id = row["id"].as<int>();
            u.username = row["username"].as<std::string>();
            u.email = row["email"].as<std::string>();
            u.phone = row["phone"].as<std::string>();
            u.password_hash = row["password_hash"].as<std::string>();
            u.role = row["role"].as<std::string>();
            u.created_at = row["created_at"].as<std::string>();
            u.last_login = row["last_login"].is_null()
                               ? "Never"
                               : row["last_login"].as<std::string>();

            users.push_back(u);
        }

    } catch (const std::exception &e) {
        std::cerr << "[DB ERROR in get_all_users]: " << e.what() << std::endl;
    }

    return users;
}

void user_repository::print_all_users() {
    auto users = get_all_users();
    for (const auto &u : users) {
        std::cout << u.id << " | " << u.username << " | " << u.email
                  << " | " << u.phone << " | " << u.role << std::endl;
    }
}

bool user_repository::entry_user(const std::string &username, const std::string &password) {
    std::cerr << "[WARNING]: entry_user is deprecated and will be removed.\n";
    return false;
}

} // namespace roomsched::db
