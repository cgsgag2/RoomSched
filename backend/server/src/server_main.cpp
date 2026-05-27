/***
 * Server main module (source).
 ***/

#include "server/app.hpp"

int main() {
    roomsched::server::app main_app;

    main_app.run(8080);
}
