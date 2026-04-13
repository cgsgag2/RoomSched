/***
 * Database user model module (header).
 ***/

#ifndef USER_MODEL_HPP_
#define USER_MODEL_HPP_

#include <ostream>
#include <string>

// project namespace, database module namespace
namespace roomsched::db {
class user {
public:
    int id;
    std::string email;
    std::string password_hash;
    std::string full_name;
    std::string phone;
    std::string role;
    std::string created_at;
    std::string last_login;
};

}  // namespace roomsched::db

#endif  // USER_MODEL_HPP_
