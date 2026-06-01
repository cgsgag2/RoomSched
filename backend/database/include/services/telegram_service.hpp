/***
 * Telegram service module (header).
 ***/

#ifndef TELEGRAM_SERVICE_HPP_
#define TELEGRAM_SERVICE_HPP_

#include <optional>
#include <string>
#include "../repositories/telegram_repository.hpp"

// project namespace, database module namespace
namespace roomsched::db {
class telegram_service {
private:
    std::string token;

public:
    telegram_service();

    bool send_message(std::int64_t chat_id, const std::string &text);
};
}  // namespace roomsched::db

#endif  // TELEGRAM_SERVICE_HPP_
