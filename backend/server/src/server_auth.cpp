/***
 * Server authentication module (source).
 ***/

#include "server_auth.hpp"
#include <string>
#include <regex>
#include "db_manager.hpp"
#include "server_json_utils.hpp"

namespace roomsched::server {

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
crow::response auth_handler::handle_login(const crow::request &req,
                                          db::database_manager &database) {
    auto body = crow::json::load(req.body);
    if (!body) {
        return crow::response(400, "Invalid JSON");
    }

    if (!body.has("email") || !body.has("password")) {
        return crow::response(400, "Missing fields");
    }

    std::string email    = body["email"].s();
    std::string password = body["password"].s();

    auto u = database.users().login(email, password);

    if (!u.has_value()) {
        return crow::response(401, "Invalid email or password");
    }

    crow::json::wvalue resp;
    resp["status"] = "success";
    resp["user"]["id"]       = u->id;
    resp["user"]["username"] = u->username;
    resp["user"]["email"]    = u->email;
    resp["user"]["phone"]    = u->phone;
    resp["user"]["role"]     = u->role;

    return crow::response(200, resp);
}

crow::response auth_handler::handle_register(const crow::request &req,
                                             db::database_manager &database) {
    auto body = crow::json::load(req.body);
    if (!body) {
        return crow::response(400, "Invalid JSON");
    }

    if (!body.has("username") || !body.has("email") ||
        !body.has("phone") || !body.has("password")) {
        return crow::response(400, "Missing fields");
    }

    std::string username = body["username"].s();
    std::string email    = body["email"].s();
    std::string phone    = body["phone"].s();
    std::string password = body["password"].s();

    if (!validate_email(email)) {
        return crow::response(400, "Invalid email format");
    }

    if (!validate_password(password)) {
        return crow::response(400, "Password too short");
    }

    bool ok = database.users().register_user(username, email, phone, password);

    if (!ok) {
        return crow::response(409, "User already exists");
    }

    crow::json::wvalue resp;
    resp["status"] = "success";
    resp["message"] = "User registered";

    return crow::response(200, resp);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
bool auth_handler::validate_email(const std::string &email) {
    static const std::regex pattern(R"((\w+)(\.{0,1})(\w*)@(\w+)\.(\w+))");
    return std::regex_match(email, pattern);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
bool auth_handler::validate_password(const std::string &password) {
    return password.size() >= 6;
}

crow::response auth_handler::get_all_users(db::database_manager &database) {
    auto users = database.users().get_all_users();

    crow::json::wvalue resp;

    for (size_t i = 0; i < users.size(); i++) {
        resp[i]["id"]       = users[i].id;
        resp[i]["username"] = users[i].username;
        resp[i]["email"]    = users[i].email;
        resp[i]["phone"]    = users[i].phone;
        resp[i]["role"]     = users[i].role;
    }

    return crow::response(200, resp);
}я

}  // namespace roomsched::server
