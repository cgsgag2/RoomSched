/***
 * Database config module (source).
 ***/

#include "core/db_config.hpp"

roomsched::db::db_config::db_config()
    : DB_NAME(get_env("DB_NAME", "roomsched")),
      DB_USER(get_env("DB_USER", "rsched_user")),
      DB_PASSWORD(get_env("DB_PASSWORD", "password")),
      DB_HOST(get_env("DB_HOST", "localhost")) {
}

roomsched::db::db_config::db_config(
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

std::string roomsched::db::db_config::get_env(
    const std::string &key,
    const std::string &default_value
) {
    const char *value = std::getenv(key.c_str());

    if (value == nullptr) {
        return default_value;
    }
    return std::string(value);
}
