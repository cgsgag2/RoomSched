#include "auth.hpp"
#include "json_utils.hpp"
#include <string>

crow::response roomsched::AuthHandler::handle_login(const crow::request& req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return roomsched::json_utils::error_response("Invalid JSON format");
    }
    
    std::vector<std::string> required = {"username", "password"};
    if (!roomsched::json_utils::validate_fields(json, required)) {
        return roomsched::json_utils::error_response("Missing username or password");
    }
    
    std::string username = json["username"].s();
    std::string password = json["password"].s();
    
    crow::json::wvalue data;
    data["token"] = "temp-token-123";
    
    return roomsched::json_utils::success_response("Login successful", data);
}

crow::response roomsched::AuthHandler::handle_register(const crow::request& req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return roomsched::json_utils::error_response("Invalid JSON format");
    }
    
    std::vector<std::string> required = {"username", "password", "email"};
    if (!roomsched::json_utils::validate_fields(json, required)) {
        return roomsched::json_utils::error_response("Missing required fields");
    }
    
    std::string username = json["username"].s();
    std::string password = json["password"].s();
    std::string email = json["email"].s();
    
    if (username.empty() || password.empty() || email.empty()) {
        return roomsched::json_utils::error_response("Fields cannot be empty");
    }
    
    if (!validate_email(email)) {
        return roomsched::json_utils::error_response("Invalid email format");
    }
    
    if (!validate_password(password)) {
        return roomsched::json_utils::error_response("Password must be at least 6 characters");
    }
    
    crow::json::wvalue data;
    data["username"] = username;
    
    return roomsched::json_utils::success_response("User registered successfully", data);
}

bool roomsched::AuthHandler::validate_email(const std::string& email) {
    int pos1 = email.find('@');
    int pos2 = email.find('.');
    if (pos1 == -1 || pos2 == -1){
        return false;
    }
    return true;
}

bool roomsched::AuthHandler::validate_password(const std::string& password) {
    return password.length() >= 6;
}