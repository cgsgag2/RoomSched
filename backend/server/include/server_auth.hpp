/***
 * Server authentication module (header).
 ***/

#ifndef SERVER_AUTH_HPP_
#define SERVER_AUTH_HPP_

#include <crow/http_response.h>
#include <crow/json.h>
#include <string>
#include "db_manager.hpp"

// project namespace, server module namespace
namespace roomsched::server {
class auth_handler {
public:
    // TODO: save only ref to db
    crow::response
    handle_register(const crow::request &req, db::database_manager &database);

    crow::response
    handle_login(const crow::request &req, db::database_manager &database);

    crow::response get_all_users(db::database_manager &database);

private:
    bool validate_email(const std::string &email);
    bool validate_password(const std::string &password);
};
}  // namespace roomsched::server

#endif  // SERVER_AUTH_HPP_
