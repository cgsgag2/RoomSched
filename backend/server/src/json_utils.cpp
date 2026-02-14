#include "json_utils.h"

namespace json_utils {
    crow::response error_response(const std::string& message, int status_code) {
        crow::json::wvalue res;
        res["status"] = "error";
        res["message"] = message;
        return crow::response(status_code, res);
    }
    
    bool validate_fields(const crow::json::rvalue& json, const std::vector<std::string>& fields) {
        for (const auto& field : fields) {
            if (!json.has(field)) {
                return false;
            }
        }
        return true;
    }
    
    crow::response success_response(const std::string& message, crow::json::wvalue data) {
        data["status"] = "success";
        data["message"] = message;
        return crow::response(200, data);
    }
}