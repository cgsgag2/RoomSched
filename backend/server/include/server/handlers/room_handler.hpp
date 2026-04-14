/***
 * Server handler rooms module (header).
 ***/

#ifndef ROOM_HANDLER_HPP_
#define ROOM_HANDLER_HPP_

#include <crow/http_response.h>
#include <crow/json.h>
#include <optional>
#include <string>
#include "db_manager.hpp"

// project namespace, server module namespace
namespace roomsched::server {
class room_handler {
public:
    explicit room_handler(db::database_manager &db_);

    crow::response get_all_rooms();
    crow::response get_room_by_id(int id);

    crow::response get_room_availability(const crow::request &req, int room_id);

private:
    db::database_manager &db;  // database manager
};
}  // namespace roomsched::server

#endif  // ROOM_HANDLER_HPP_
