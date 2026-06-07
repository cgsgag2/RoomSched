/***
 * Telegram service module (source).
 ***/

#include <cpr/cpr.h>
#include <chrono>
#include <exception>
#include <iostream>
#include <nlohmann/json.hpp>
#include <random>
#include <string>
#include <thread>
// Local includes
#include "services/telegram_service.hpp"

namespace roomsched::db {

telegram_service::telegram_service(database &db_)
    : tg_repo(db_), user_repo(db_) {
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

std::string telegram_service::generate_link_code() {
    static const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dist(0, chars.size() - 1);

    std::string code;

    for (int i = 0; i < 6; i++) {
        code += chars[dist(gen)];
    }

    return code;
}

void telegram_service::start_polling() {
    std::cout << "[TELEGRAM] polling started" << std::endl;

    while (true) {
        try {
            const std::string url =
                "https://api.telegram.org/bot" + token + "/getUpdates";

            auto response = cpr::Get(
                cpr::Url{url},
                cpr::Parameters{{"offset", std::to_string(last_update_id + 1)}}
            );

            if (response.status_code != 200) {
                std::cerr << "[TELEGRAM] getUpdates failed. Status: "
                          << response.status_code << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3));

                continue;
            }

            auto json = nlohmann::json::parse(response.text);
            if (!json.contains("ok") || !json["ok"].get<bool>()) {
                std::this_thread::sleep_for(std::chrono::seconds(3));

                continue;
            }

            auto updates = json["result"];

            for (const auto &update : updates) {
                std::int64_t update_id =
                    update["update_id"].get<std::int64_t>();

                last_update_id = update_id;

                if (!update.contains("message")) {
                    continue;
                }

                const auto message = update["message"];
                if (!message.contains("text")) {
                    continue;
                }

                std::string text = message["text"].get<std::string>();
                std::int64_t chat_id =
                    message["chat"]["id"].get<std::int64_t>();

                process_message(text, chat_id);
            }
        } catch (const std::exception &e) {
            std::cerr << "[TELEGRAM ERROR] TELEGRAM POLLING ERROR" << e.what()
                      << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

void telegram_service::process_message(
    const std::string &text,
    std::int64_t chat_id
) {
    // We expect '/link ABCD12'
    if (!text.starts_with("/link ")) {
        return;
    }

    std::string code = text.substr(6);
    auto user_id = get_user_id_by_code(code);
    if (!user_id) {
        send_message(chat_id, "Invalid code");
        return;
    }

    bool success = user_repo.set_telegram_chat_id(*user_id, chat_id);
    if (!success) {
        send_message(chat_id, "Failed to link account");
        return;
    }

    delete_link(code);
    send_message(chat_id, "Telegram linked successfully!");

    std::cout << "[TELEGRAM] linker user " << *user_id << " with chat_id "
              << chat_id << " successfully" << std::endl;
}

// Repository (for service) module

bool telegram_service::create_link(const std::string &code, int user_id) {
    telegram_link tg;
    tg.code = code;
    tg.user_id = user_id;

    return tg_repo.create_link(tg);
}

bool telegram_service::delete_link(const std::string &code) {
    return tg_repo.delete_link(code);
}

std::optional<int> telegram_service::get_user_id_by_code(const std::string &code
) {
    return tg_repo.get_user_id_by_code(code);
}

}  // namespace roomsched::db
