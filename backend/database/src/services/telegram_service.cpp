/***
 * Telegram service module (source).
 ***/

#include <cpr/cpr.h>
#include <exception>
#include <iostream>
#include <string>
// Local includes
#include "services/telegram_service.hpp"

namespace roomsched::db {

telegram_service::telegram_service() {
    const char *env_token = std::getenv("TELEGRAM_BOT_TOKEN");

    if (env_token == nullptr) {
        throw std::runtime_error("[TELEGRAM ERROR] TELEGRAM_BOT_TOKEN not set");
    }
    token = std::string(env_token);
}

bool telegram_service::send_message(
    std::int64_t chat_id,
    const std::string &text
) {
    try {
        const std::string url =
            "https://api.telegram.org/bot" + token + "/sendMessage";

        auto response = cpr::Post(
            cpr::Url{url},
            cpr::Payload{{"chat_id", std::to_string(chat_id)}, {"text", text}}
        );

        if (response.status_code != 200) {
            std::cerr << "[TELEGRAM ERROR] HTTP status: "
                      << response.status_code << std::endl;

            std::cerr << response.text << std::endl;

            return false;
        }

        return true;
    } catch (const std::exception &e) {
        std::cerr << "[TELEGRAM ERROR] " << e.what() << std::endl;

        return false;
    }
}

}  // namespace roomsched::db
