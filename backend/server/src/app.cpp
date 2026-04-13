/***
 * Server app module (source).
 ***/

#include <iostream>
// Local includes
#include "server/app.hpp"
#include "server/router.hpp"

namespace roomsched::server {

app::app() : db_manager(db::db_config{}), auth(db_manager) {
    server_app.loglevel(crow::LogLevel::Debug);

    setup_routes();
}

db::database_manager &app::get_db() {
    return db_manager;
}

auth_handler &app::get_auth() {
    return auth;
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
