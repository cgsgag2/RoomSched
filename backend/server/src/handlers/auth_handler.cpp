/***
 * Server handler authentication module (source).
 ***/

#include <regex>
#include <string>
// Local includes
#include "db_manager.hpp"
#include "server/handlers/auth_handler.hpp"
#include "server/utils/json_utils.hpp"

namespace roomsched::server {

auth_handler::auth_handler(db::database_manager &db_) : db(db_) {
}

crow::response auth_handler::register_user(const crow::request &req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return crow::response(400, "Invalid JSON");
    }

    if (!json.has("email") || !json.has("password") || !json.has("fullname") ||
        !json.has("phone")) {
        return crow::response(400, "Missing fields in json data");
    }

    std::string email = json["email"].s();
    std::string password = json["password"].s();
    std::string fullname = json["fullname"].s();
    std::string phone = json["phone"].s();

    if (!validate_email(email)) {
        return crow::response(400, "Invalid email format");
    }

    bool is_registered =
        db.users().register_user(email, password, fullname, phone);

    if (!is_registered) {
        std::cerr << "[AUTH]: registration failed for email: " << email
                  << std::endl;
        return crow::response(409, "Some problems in registration NEW user");
    }

    crow::json::wvalue resp;
    resp["status"] = "success";
    resp["message"] = "User registered";

    std::cout << "[AUTH]: registration success: " << email << std::endl;
    return crow::response(200, resp);
}

crow::response auth_handler::login(const crow::request &req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return crow::response(400, "Invalid JSON");
    }

    if (!json.has("email") || !json.has("password")) {
        return crow::response(400, "Missing fields");
    }

    std::string email = json["email"].s();
    std::string password = json["password"].s();

    auto u = db.users().login(email, password);

    if (!u.has_value()) {
        std::cerr << "[AUTH]: login failed for email: " << email << std::endl;
        return crow::response(401, "Invalid email or password");
    }

    crow::json::wvalue resp;
    resp["status"] = "success";
    resp["user"]["id"] = u->id;
    resp["user"]["full_name"] = u->full_name;
    resp["user"]["email"] = u->email;
    resp["user"]["phone"] = u->phone;
    resp["user"]["role"] = u->role;

    std::cout << "[AUTH]: login success: " << email << std::endl;
    return crow::response(200, resp);
}

bool auth_handler::validate_email(const std::string &email) {
    static const std::regex pattern(R"((\w+)(\.{0,1})(\w*)@(\w+)\.(\w+))");
    return std::regex_match(email, pattern);
}

crow::response auth_handler::get_all_users() {
    auto users = db.users().get_all_users();

    crow::json::wvalue resp;

    for (size_t i = 0; i < users.size(); i++) {
        resp[i]["id"] = users[i].id;
        resp[i]["full_name"] = users[i].full_name;
        resp[i]["email"] = users[i].email;
        resp[i]["phone"] = users[i].phone;
        resp[i]["role"] = users[i].role;
    }

    return crow::response(200, resp);
}

}  // namespace roomsched::server
