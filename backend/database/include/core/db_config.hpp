/***
 * Database config module (header).
 ***/

#ifndef DB_CONFIG_HPP_
#define DB_CONFIG_HPP_

#include <string>
#include <utility>

// project namespace, database module namespace
namespace roomsched::db {
class db_config {
public:
    const std::string DB_NAME = "roomsched";
    const std::string DB_USER = "rsched_user";
    const std::string DB_PASSWORD = "RschedUser87204576";
    const std::string DB_HOST = "localhost";

    db_config() = default;

    db_config(
        std::string name,
        std::string user,
        std::string pwd,
        std::string host
    )
        : DB_NAME(std::move(name)),
          DB_USER(std::move(user)),
          DB_PASSWORD(std::move(pwd)),
          DB_HOST(std::move(host)) {
    }
};
}  // namespace roomsched::db

#endif  // DB_CONFIG_HPP_
