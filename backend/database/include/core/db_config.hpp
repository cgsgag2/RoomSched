/***
 * Database config module (header).
 ***/

#ifndef DB_CONFIG_HPP_
#define DB_CONFIG_HPP_

#include <cstdlib>
#include <string>
#include <utility>

// project namespace, database module namespace
namespace roomsched::db {
class db_config {
public:
    const std::string DB_NAME;
    const std::string DB_USER;
    const std::string DB_PASSWORD;
    const std::string DB_HOST;

    db_config();

    db_config(
        std::string name,
        std::string user,
        std::string pwd,
        std::string host
    );

private:
    static std::string
    get_env(const std::string &key, const std::string &default_value);
};
}  // namespace roomsched::db

#endif  // DB_CONFIG_HPP_
