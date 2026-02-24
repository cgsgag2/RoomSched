/***
 * Server json utilities module (source).
 ***/

#include "server_json_utils.hpp"

crow::response roomsched::server::json_utils::error_response(
    const std::string &message,
    int status_code
) {
    crow::json::wvalue res;
    res["status"] = "error";
    res["message"] = message;
    return {status_code, res};
}

bool roomsched::server::json_utils::validate_fields(
    const crow::json::rvalue &json,
    const std::vector<std::string> &fields
) {
    // NOLINTNEXTLINE(readability-use-anyofallof)
    for (const auto &field : fields) {
        if (!json.has(field)) {
            return false;
        }
    }
    return true;
}

crow::response roomsched::server::json_utils::success_response(
    const std::string &message,
    crow::json::wvalue data
) {
    data["status"] = "success";
    data["message"] = message;
    return {200, data};
}
