/***
 * Server handler booking module (header).
 ***/

#ifndef BOOKING_HANDLER_HPP_
#define BOOKING_HANDLER_HPP_

#if 0

#include <crow/http_response.h>
#include <crow/json.h>
#include <string>
#include "db_manager.hpp"

// project namespace, server module namespace
namespace roomsched::server {
class bookings_handler {
public:
    crow::response
    create_booking(const crow::request &req, db::database_manager &database);

    crow::response
    cancel_booking(int booking_id, db::database_manager &database);

    crow::response
    get_user_bookings(int user_id, db::database_manager &database);

    crow::response create_availability(
        const crow::request &req,
        db::database_manager &database,
        int room_id
    );
};
}  // namespace roomsched::server

#endif  // 0

#endif  // BOOKING_HANDLER_HPP_
