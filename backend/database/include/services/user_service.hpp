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
enum class register_error {
    none,
    weak_password,
    email_exists,
    phone_exists,
    fullname_exists,
    db_error
};

struct register_result {
    bool success;
    register_error error;
};

class user_service {
private:
    user_repository repo;

public:
    user_service(database &db_);

    register_result register_user(
        const std::string &email,
        const std::string &password,
        const std::string &full_name,
        const std::string &phone
    );

    std::optional<user>
    login(const std::string &email, const std::string &password);

    std::vector<user> get_all_users();

    bool email_exists(const std::string &email);

    std::optional<std::int64_t> get_telegram_chat_id(int user_id);

private:
    std::string hash_password(const std::string &password);
    bool verify_password(const std::string &password, const std::string &hash);
    bool is_password_strong(const std::string &password);
};
}  // namespace roomsched::db

#endif  // USER_SERVICE_HPP_
