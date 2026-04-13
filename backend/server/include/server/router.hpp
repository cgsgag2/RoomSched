/***
 * Server router module (header).
 ***/

#ifndef ROUTER_HPP_
#define ROUTER_HPP_

#include <crow/app.h>

// TODO: think abot it vs forward declaration
// #include "server/app.hpp"

// project namespace, server module namespace
namespace roomsched::server {

// Forward declaration
class app;

void setup_all_routes(crow::SimpleApp &app_ref, app &server);

}  // namespace roomsched::server

#endif  // ROUTER_HPP_
