/***
 * Server authentication module (source).
 ***/

#include "server_auth.hpp"
#include <string>
#include "db_manager.hpp"
#include "server_json_utils.hpp"

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
crow::response roomsched::server::auth_handler::handle_login(
    const crow::request &req,
    db::database_manager &database
) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return json_utils::error_response("Invalid JSON format");
    }

    const std::vector<std::string> required = {"username", "password"};
    if (!json_utils::validate_fields(json, required)) {
        return json_utils::error_response("Missing username or password");
    }

    // cppcheck-suppress unreadVariable
    const std::string username = json["username"].s();
    // cppcheck-suppress unreadVariable
    const std::string password = json["password"].s();

    // TODO: add to DB username, password

    // TODO: think about tokens
    crow::json::wvalue data;
    data["token"] = "temp-token-123";

    return json_utils::success_response("Login successful", data);
}

crow::response roomsched::server::auth_handler::handle_register(
    const crow::request &req,
    db::database_manager &database
) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return json_utils::error_response("Invalid JSON format");
    }

    const std::vector<std::string> required = {"username", "password", "email"};
    if (!json_utils::validate_fields(json, required)) {
        return json_utils::error_response("Missing required fields");
    }

    const std::string username = json["username"].s();
    const std::string password = json["password"].s();
    const std::string email = json["email"].s();

    if (username.empty() || email.empty()) {
        return json_utils::error_response("Fields cannot be empty");
    }

    if (!validate_email(email)) {
        return json_utils::error_response("Invalid email format");
    }

    /*
   if (!validate_password(password)) {
       return json_utils::error_response(
           "Password must be at least 6 characters"
       );
   } */

    // CHECK: add first login: username and password
    database.create_user(username, password);

    // TODO: check if user exists

    // TODO: think about tokens
    crow::json::wvalue data;
    data["username"] = username;

    return json_utils::success_response("User registered successfully", data);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
bool roomsched::server::auth_handler::validate_email(const std::string &email) {
    const auto pos1 = email.find('@');
    const auto pos2 = email.find('.');

    return pos1 != -1 && pos2 != -1;
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
bool roomsched::server::auth_handler::validate_password(
    const std::string &password
) {
    return password.length() >= 6;
}

crow::response roomsched::server::auth_handler::get_all_users(
    db::database_manager &database
) {
    std::vector<db::user> all_users = database.get_all_users();

    // Note: crow::json::wvalue::list = std::vector<crow::json::wvalue>
    crow::json::wvalue::list users_format_crow{};
    for (std::size_t i = 0; i < all_users.size(); i++) {
        users_format_crow.push_back(
            {{"id", all_users[i].id},
             {"username", all_users[i].username},
             {"created_at", all_users[i].created_at},
             {"last_login", all_users[i].last_login}}
        );
    }

    crow::json::wvalue data;
    data["users"] = crow::json::wvalue::list(users_format_crow);

    return json_utils::success_response("Get users from DB", data);
}
