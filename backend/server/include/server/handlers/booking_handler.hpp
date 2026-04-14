/***
 * Server handler booking module (header).
 ***/

#ifndef BOOKING_HANDLER_HPP_
#define BOOKING_HANDLER_HPP_

#include <crow/http_response.h>
#include <crow/json.h>
#include <optional>
#include <string>
#include "db_manager.hpp"

// project namespace, server module namespace
namespace roomsched::server {
class bookings_handler {
public:
    explicit bookings_handler(db::database_manager &db_);

    crow::response create_booking(const crow::request &req);
    crow::response cancel_booking(int booking_id);

    crow::response get_all_bookings();
    crow::response get_bookings_by_user(int user_id);
    crow::response get_bookings_by_room(int room_id);

private:
    db::database_manager &db;  // database manager
};
}  // namespace roomsched::server

#endif  // BOOKING_HANDLER_HPP_
