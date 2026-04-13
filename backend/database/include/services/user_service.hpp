/***
 * Database user service module (header).
 ***/

#ifndef USER_SERVICE_HPP_
#define USER_SERVICE_HPP_

#include <optional>
#include <string>
#include "../repositories/user_repository.hpp"

// project namespace, database module namespace
namespace roomsched::db {
class user_service {
private:
    user_repository repo;

public:
    user_service(database &db_);

    bool register_user(
        const std::string &email,
        const std::string &password,
        const std::string &full_name,
        const std::string &phone
    );

    std::optional<user>
    login(const std::string &email, const std::string &password);

    std::vector<user> get_all_users();

    bool email_exists(const std::string &email);

private:
    std::string hash_password(const std::string &password);
    bool verify_password(const std::string &password, const std::string &hash);
    bool is_password_strong(const std::string &password);
};
}  // namespace roomsched::db

#endif  // USER_SERVICE_HPP_
