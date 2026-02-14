#pragma once
#include "crow/http_response.h"
#include "crow/json.h"
#include <string>
#include <vector>

namespace json_utils {
    crow::response error_response(const std::string& message, int status_code = 400);
    bool validate_fields(const crow::json::rvalue& json, const std::vector<std::string>& fields);
    crow::response success_response(const std::string& message, crow::json::wvalue data = {});
}