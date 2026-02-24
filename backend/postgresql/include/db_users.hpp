/***
 * Database users module (header).
 ***/

#ifndef DB_USERS_HPP_
#define DB_USERS_HPP_

#include <ostream>
#include <string>

// project namespace, database module namespace
namespace roomsched::db {
class user {
public:
    int id;
    std::string username;
    std::string password;  // TODO: hash
    std::string created_at;
    std::string last_login;
};

std::ostream &operator<<(std::ostream &os, const user &user);
}  // namespace roomsched::db

#endif  // DB_USERS_HPP_
