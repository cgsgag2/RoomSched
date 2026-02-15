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
    app_db.createUser("Sanya", "password123");
    app_db.createUser("Diana", "pwd_dn");
    app_db.createUser("Varvara", "secure_789");

    // Try to create user that already exists
    app_db.createUser("Sanya", "password123");

    // Print all users
    app_db.printAllUsers();
    // Authenticate new user (update info)
    std::cout << "\n========== Check authentication ==========\n";
    std::string u_username, u_password;

    std::cout << "Input login: ";
    std::cin >> u_username;
    std::cout << "Input password: ";
    std::cin >> u_password;

    if (app_db.entryUser(u_username, u_password)) {
        std::cout << "Login successfully! Hi, " << u_username << "!"
                  << std::endl;
    } else {
        std::cout << "Incorrect login/password" << std::endl;
    }

    // last_login was updated
    app_db.printAllUsers();

    /// OLD TEST
    // std::cout << "First attempt to connect to DB" << std::endl;

    // roomsched::database_main my_db(
    //     "localhost", "5432", "db_check_connection_1", "user_alex", "password"
    // );
    // my_db.try_connection();

    // std::vector<roomsched::data_base_info> info = my_db.getInfoFromDB();
    // for (const auto &x : info) {
    //     std::cout << "==================\n";
    //     x.print();
    //     std::cout << std::endl;
    // }
    return 0;
}
