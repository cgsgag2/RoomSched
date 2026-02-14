#ifndef AUTH_HPP
#define AUTH_HPP
#include "crow/http_response.h"
#include "crow/json.h"
#include <string>

namespace roomsched {
class AuthHandler {
public:
    crow::response handle_register(const crow::request& req);
    crow::response handle_login(const crow::request& req);
private:
    bool validate_email(const std::string& email);
    bool validate_password(const std::string& password);
};
} // namespace roomsched
#endif