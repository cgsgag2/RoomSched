/***
 * DB main module.
 ***/

#include <iostream>
#include <pqxx/pqxx>
#include <vector>
#include "include/db_manager.hpp"

int main() {
    roomsched::db::database_manager app_db;

    // Init DB (and check connection)
    if (!app_db.init()) {
        std::cerr << "STOP PROGRAM." << std::endl;
        return 0;
    }

    // Create new users (example)
    std::cout << "\n========== Create new users ==========\n";
    app_db.create_user("Sanya", "password123");
    app_db.create_user("Diana", "pwd_dn");
    app_db.create_user("Varvara", "secure_789");

    // Try to create user that already exists
    app_db.create_user("Sanya", "password123");

    // Print all users
    app_db.print_all_users();
    // Authenticate new user (update info)
    std::cout << "\n========== Check authentication ==========\n";
    std::string u_username;
    std::string u_password;

    std::cout << "Input login: ";
    std::cin >> u_username;
    std::cout << "Input password: ";
    std::cin >> u_password;

    if (app_db.entry_user(u_username, u_password)) {
        std::cout << "Login successfully! Hi, " << u_username << "!"
                  << std::endl;
    } else {
        std::cout << "Incorrect login/password" << std::endl;
    }

    // last_login was updated
    app_db.print_all_users();
    // std::vector<roomsched::db::user> users = app_db.get_all_users();
    // for (auto u : users) {
    //     std::cout << u;
    // }

    return 0;
}
