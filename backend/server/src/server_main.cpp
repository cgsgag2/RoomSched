/***
 * Server main module (source).
 ***/

#include <thread>
#include "server/app.hpp"

int main() {
    roomsched::server::app main_app;

    std::thread([&main_app]() { main_app.get_db().telegram().start_polling(); }
    ).detach();

    main_app.run(8080);
}
