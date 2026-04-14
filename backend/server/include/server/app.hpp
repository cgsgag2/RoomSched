/***
 * Server app module (header).
 ***/

#ifndef APP_HPP_
#define APP_HPP_

#include <crow/app.h>
#include "db_manager.hpp"
#include "server/handlers/auth_handler.hpp"
#include "server/handlers/booking_handler.hpp"
#include "server/handlers/buildings_handler.hpp"
#include "server/handlers/room_handler.hpp"

// project namespace, server module namespace
namespace roomsched::server {
class app {
private:
    db::database_manager db_manager;
    crow::SimpleApp server_app;

    auth_handler authHandler;
    room_handler roomHandler;
    bookings_handler bookingHandler;
    // buildings_handler buildings;

    void setup_routes();

public:
    app();

    db::database_manager &get_db();
    auth_handler &get_auth_handler();
    room_handler &get_room_handler();
    bookings_handler &get_booking_handler();
    // buildings_handler &get_buildings();

    void run(int port);
};
}  // namespace roomsched::server

#endif  // APP_HPP_
