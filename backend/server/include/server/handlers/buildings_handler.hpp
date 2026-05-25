/***
 * Server handler buildings module (header).
 ***/

#ifndef BUILDINGS_HANDLER_HPP_
#define BUILDINGS_HANDLER_HPP_

#include <crow/http_response.h>
#include <crow/json.h>
#include "db_manager.hpp"

// project namespace, server module namespace
namespace roomsched::server {

class buildings_handler {
private:
    db::database_manager &db;

public:
    explicit buildings_handler(db::database_manager &db_);

    crow::response get_all_buildings();
};

}  // namespace roomsched::server

#endif  // BUILDINGS_HANDLER_HPP_
