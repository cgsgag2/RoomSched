/***
 * Server handler authentication module (header).
 ***/

#ifndef AUTH_HANDLER_HPP_
#define AUTH_HANDLER_HPP_

#include <crow/http_response.h>
#include <crow/json.h>
#include <optional>
#include <string>
#include "db_manager.hpp"

// project namespace, server module namespace
namespace roomsched::server {
class auth_handler {
public:
    explicit auth_handler(db::database_manager &db_);

    crow::response register_user(const crow::request &req);
    crow::response login(const crow::request &req);

    crow::response get_all_users();

private:
    db::database_manager &db;  // database manager

    bool validate_email(const std::string &email);
};
}  // namespace roomsched::server

#endif  // AUTH_HANDLER_HPP_
