/***
 * Server app module (source).
 ***/

#include <iostream>
// Local includes
#include "server/app.hpp"
#include "server/router.hpp"

namespace roomsched::server {

app::app()
    : db_manager(db::db_config{}),
      authHandler(db_manager),
      roomHandler(db_manager),
      bookingHandler(db_manager) {
    // server_app.loglevel(crow::LogLevel::Debug);

    db_manager.rooms().create_default_rooms();
    setup_routes();
}

db::database_manager &app::get_db() {
    return db_manager;
}

auth_handler &app::get_auth_handler() {
    return authHandler;
}

room_handler &app::get_room_handler() {
    return roomHandler;
}

bookings_handler &app::get_booking_handler() {
    return bookingHandler;
}

void app::run(int port) {
    std::cout << "SERVER STARTED SUCCESSFULLY" << std::endl;
    // TODO: think about it!
    // server_app.port(port).multithreaded().run();
    server_app.port(port).run();
}

void app::setup_routes() {
    setup_all_routes(server_app, *this);
}
}  // namespace roomsched::server
