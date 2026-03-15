/***
 * Database user service module (source).
 ***/

#include "../../include/services/user_service.hpp"
#include <iostream>
#include <bcrypt/BCrypt.hpp> 

namespace roomsched::db {

user_service::user_service(user_repository &repo_) : repo(repo_) {}

std::string user_service::hash_password(const std::string &password) {
    return BCrypt::generateHash(password);
}

bool user_service::verify_password(const std::string &password,
                                   const std::string &hash) {
    return BCrypt::validatePassword(password, hash);
}

bool user_service::register_user(const std::string &username,
                                 const std::string &email,
                                 const std::string &phone,
                                 const std::string &password) {

    if (repo.user_exists_by_email(email)) {
        std::cerr << "[SERVICE]: email already exists\n";
        return false;
    }

    if (repo.user_exists_by_phone(phone)) {
        std::cerr << "[SERVICE]: phone already exists\n";
        return false;
    }

    std::string hash = hash_password(password);
    user new_user;
    new_user.username = username;
    new_user.email = email;
    new_user.phone = phone;
    new_user.password_hash = hash;
    new_user.role = "user";

    return repo.create_user(new_user);
}

std::optional<user> user_service::login(const std::string &email,
                                        const std::string &password) {

    auto u = repo.get_user_by_email(email);
    if (!u.has_value()) {
        std::cerr << "[SERVICE]: user not found\n";
        return std::nullopt;
    }

    if (!verify_password(password, u->password_hash)) {
        std::cerr << "[SERVICE]: wrong password\n";
        return std::nullopt;
    }

    return u;
}

std::vector<user> user_service::get_all_users() {
    return repo.get_all_users();
}

} // namespace roomsched::db