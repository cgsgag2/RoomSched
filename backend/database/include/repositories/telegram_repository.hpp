/***
 * Database telegram repository module (header).
 ***/

#ifndef TELEGRAM_REPOSITORY_HPP_
#define TELEGRAM_REPOSITORY_HPP_

#include <optional>
#include "../core/db_core.hpp"
#include "../models/telegram_link_model.hpp"

// project namespace, database module namespace
namespace roomsched::db {
class telegram_repository {
private:
    database &db;

public:
    telegram_repository(database &db_);

    bool create_link(const telegram_link &tg);
    bool delete_link(const std::string &code);

    std::optional<int> get_user_id_by_code(const std::string &code);
};
}  // namespace roomsched::db

#endif  // TELEGRAM_REPOSITORY_HPP_
