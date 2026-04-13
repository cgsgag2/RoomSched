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

// project namespace, server module namespace
namespace roomsched::server {
class app {
private:
    db::database_manager db_manager;
    crow::SimpleApp server_app;

    auth_handler auth;
    // bookings_handler bookings;
    // buildings_handler buildings;

    void setup_routes();

public:
    app();

    db::database_manager &get_db();
    auth_handler &get_auth();
    // bookings_handler &get_bookings();
    // buildings_handler &get_buildings();

    void run(int port);
};
}  // namespace roomsched::server

#endif  // APP_HPP_
