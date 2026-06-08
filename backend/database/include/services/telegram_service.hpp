/***
 * Telegram service module (header).
 ***/

#ifndef TELEGRAM_SERVICE_HPP_
#define TELEGRAM_SERVICE_HPP_

#include <atomic>
#include <optional>
#include <string>
#include "../repositories/telegram_repository.hpp"
#include "../repositories/user_repository.hpp"

// project namespace, database module namespace
namespace roomsched::db {
class telegram_service {
private:
    telegram_repository tg_repo;
    user_repository user_repo;

    std::string token;

    std::atomic<std::int64_t> last_update_id{0};

    void process_message(const std::string &text, std::int64_t chat_id);

public:
    telegram_service(database &db_);

    // Telegram API (api.telegram.org)
    bool send_message(std::int64_t chat_id, const std::string &text);

    std::string generate_link_code();

    void start_polling();

    // Telegram repository connection
    bool create_link(const std::string &code, int user_id);
    bool delete_link(const std::string &code);

    std::optional<int> get_user_id_by_code(const std::string &code);
};
}  // namespace roomsched::db

#endif  // TELEGRAM_SERVICE_HPP_
