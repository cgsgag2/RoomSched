/***
 * Database manager module (header).
 ***/

#ifndef DB_MANAGER_HPP_
#define DB_MANAGER_HPP_

#include <memory>
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include "db_users.hpp"

// project namespace, database module namespace
namespace roomsched::db {
class database_manager {
private:
    std::unique_ptr<pqxx::connection> conn;
    const std::string DB_NAME = "roomsched";
    const std::string DB_USER = "rsched_user";
    const std::string DB_PASSWORD = "RschedUser87204576";
    const std::string DB_HOST = "localhost";

public:
    // TODO: Log file (to execute all comands from console) - ?
    bool init();

    bool create_user(const std::string &username, const std::string &password);

    void print_all_users();

    std::vector<user> get_all_users();

    bool entry_user(const std::string &username, const std::string &password);
};
}  // namespace roomsched::db

#endif  // DB_MANAGER_HPP_
