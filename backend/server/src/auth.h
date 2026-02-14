#pragma once
#include "crow/http_response.h"
#include "crow/json.h"
#include <string>

crow::response handle_register(const crow::request& req);
crow::response handle_login(const crow::request& req);
bool validate_email(const std::string& email);
bool validate_password(const std::string& password);