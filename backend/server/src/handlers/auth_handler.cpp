/***
 * Server handler authentication module (source).
 ***/

#include <regex>
#include <string>
// Local includes
#include "db_manager.hpp"
#include "server/handlers/auth_handler.hpp"
#include "server/utils/error_codes.hpp"
#include "server/utils/json_utils.hpp"

namespace roomsched::server {

auth_handler::auth_handler(db::database_manager &db_) : db(db_) {
}

crow::response auth_handler::register_user(const crow::request &req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return json_utils::error_response(
            "Invalid JSON",
            400,
            error_codes::kInvalidJson
        );
    }

    if (!json_utils::validate_fields(
            json,
            {"email", "password", "fullname", "phone"}
        )) {
        return json_utils::error_response(
            "Missing fields in json data",
            400,
            error_codes::kMissingFields
        );
    }

    std::string email = json["email"].s();
    std::string password = json["password"].s();
    std::string fullname = json["fullname"].s();
    std::string phone = json["phone"].s();

    if (!validate_email(email)) {
        return json_utils::error_response(
            "Invalid email format",
            400,
            error_codes::kInvalidEmail
        );
    }

    auto result = db.users().register_user(email, password, fullname, phone);

    if (!result.success) {
        std::cerr << "[AUTH]: registration failed for email: " << email
                  << std::endl;
        switch (result.error) {
            case db::register_error::weak_password:
                return json_utils::error_response(
                    "Password is too weak",
                    422,
                    error_codes::kWeakPassword
                );
            case db::register_error::email_exists:
                return json_utils::error_response(
                    "Email already exists",
                    409,
                    error_codes::kEmailExists
                );
            case db::register_error::phone_exists:
                return json_utils::error_response(
                    "Phone already exists",
                    409,
                    error_codes::kPhoneExists
                );
            case db::register_error::fullname_exists:
                return json_utils::error_response(
                    "Fullname already exists",
                    409,
                    error_codes::kFullnameExists
                );
            default:
                return json_utils::error_response(
                    "Some problems in registration new user",
                    500,
                    error_codes::kRegistrationFailed
                );
        }
    }

    std::cout << "[AUTH]: registration success: " << email << std::endl;
    crow::json::wvalue resp;
    resp["status"] = "success";
    resp["message"] = "User registered";
    return crow::response(200, resp);
}

crow::response auth_handler::login(const crow::request &req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return json_utils::error_response(
            "Invalid JSON",
            400,
            error_codes::kInvalidJson
        );
    }

    if (!json_utils::validate_fields(json, {"email", "password"})) {
        return json_utils::error_response(
            "Missing fields",
            400,
            error_codes::kMissingFields
        );
    }

    std::string email = json["email"].s();
    std::string password = json["password"].s();

    auto u = db.users().login(email, password);

    if (!u.has_value()) {
        std::cerr << "[AUTH]: login failed for email: " << email << std::endl;
        return json_utils::error_response(
            "Invalid email or password",
            401,
            error_codes::kInvalidCredentials
        );
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
