/***
 * Server telegram handler module (source).
 ***/

#include "server/handlers/telegram_handler.hpp"
#include "db_manager.hpp"
#include "server/utils/error_codes.hpp"
#include "server/utils/json_utils.hpp"

namespace roomsched::server {

telegram_handler::telegram_handler(db::database_manager &db_) : db(db_) {
}

crow::response telegram_handler::link_telegram(const crow::request &req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return json_utils::error_response(
            "Invalid JSON", 400, error_codes::kInvalidJson
        );
    }

    if (!json_utils::validate_fields(json, {"user_id"})) {
        return json_utils::error_response(
            "Missing fields", 400, error_codes::kMissingFields
        );
    }

    int user_id = json["user_id"].i();

    const std::string code = db.telegram().generate_link_code();
    bool success = db.telegram().create_link(code, user_id);
    if (!success) {
        std::cerr << "[TELEGRAM]: failed to link user: " << user_id
                  << std::endl;
        return json_utils::error_response(
            "Invalid user id", 500, error_codes::kInvalidCredentials
        );
    }

    crow::json::wvalue resp;
    resp["status"] = "success";
    resp["code"] = code;

    std::cout << "[TELEGRAM]: link success: " << code << std::endl;
    return crow::response(200, resp);
}

}  // namespace roomsched::server
