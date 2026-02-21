/***
 * Database manager module (header).
 ***/

#ifndef DB_MANAGER_HPP_
#define DB_MANAGER_HPP_

#include <memory>
#include <pqxx/pqxx>
#include <string>

// project namespace
namespace roomsched {
// database module namespace
namespace db {
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

    bool createUser(const std::string &username, const std::string &password);

    void printAllUsers();

    bool entryUser(const std::string &username, const std::string &password);
};
}  // namespace db
}  // namespace roomsched

#endif  // DB_MANAGER_HPP_
