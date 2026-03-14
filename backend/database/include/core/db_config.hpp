/***
 * Database config module (header).
 ***/

#ifndef DB_CONFIG_HPP_
#define DB_CONFIG_HPP_

#include <string>

// project namespace, database module namespace
namespace roomsched::db {
class db_config {
public:
    const std::string DB_NAME = "roomsched";
    const std::string DB_USER = "rsched_user";
    const std::string DB_PASSWORD = "RschedUser87204576";
    const std::string DB_HOST = "localhost";
};
}  // namespace roomsched::db

#endif  // DB_CONFIG_HPP_
