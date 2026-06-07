/***
 * Server telegram handler module (header).
 ***/

#ifndef TELEGRAM_HANDLER_HPP_
#define TELEGRAM_HANDLER_HPP_

#include <crow/http_response.h>
#include <crow/json.h>
#include <optional>
#include <string>
#include "db_manager.hpp"

// project namespace, server module namespace
namespace roomsched::server {
class telegram_handler {
public:
    explicit telegram_handler(db::database_manager &db_);

    crow::response link_telegram(const crow::request &req);

private:
    db::database_manager &db;  // database manager
};
}  // namespace roomsched::server

#endif  // TELEGRAM_HANDLER_HPP_
