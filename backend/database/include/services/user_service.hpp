/***
 * Database user service module (header).
 ***/

#ifndef USER_SERVICE_HPP_
#define USER_SERVICE_HPP_

#include "../repositories/user_repository.hpp"

// project namespace, database module namespace
namespace roomsched::db {
class user_service {
private:
    user_repository repo;

public:
    user_service(database &db_);

    // TODO: user module!!!
    void register_user(const user &new_user);

    user get_user_by_id(int id);
    std::vector<user> get_all_users();
    /*
    void print_all_users();

    bool entry_user(const std::string &username, const std::string &password);
    */
};
}  // namespace roomsched::db

#endif  // USER_SERVICE_HPP_
