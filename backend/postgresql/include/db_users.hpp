/***
 * Database users module (header).
 ***/

#ifndef DB_USERS_HPP_
#define DB_USERS_HPP_

#include <ostream>
#include <string>

// project namespace
namespace roomsched {
// database module namespace
namespace db {
class user {
public:
    int id;
    std::string username;
    std::string password;  // TODO: hash
    std::string created_at;
    std::string last_login;
};

std::ostream &operator<<(std::ostream &os, const user &user);
}  // namespace db
}  // namespace roomsched

#endif  // DB_USERS_HPP_
