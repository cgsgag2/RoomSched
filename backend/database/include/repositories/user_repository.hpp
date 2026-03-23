/***
 * Database user repository module (header).
 ***/

#ifndef USER_REPOSITORY_HPP_
#define USER_REPOSITORY_HPP_

#include "../core/db_core.hpp"
#include "../models/user_model.hpp"

// project namespace, database module namespace
namespace roomsched::db {
class user_repository {
private:
    database &db;

public:
    user_repository(database &db_);

    bool create_user(const user &new_user);

    bool user_exists_by_email(const std::string& email);
    bool user_exists_by_phone(const std::string& phone);

    user get_user_by_id(int id);
    std::optional<user> get_user_by_email(const std::string& email);
    std::vector<user> get_all_users();
    void print_all_users();

    bool entry_user(const std::string &username, const std::string &password);
};
}  // namespace roomsched::db

#endif  // USER_REPOSITORY_HPP_
