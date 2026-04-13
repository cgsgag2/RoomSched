/***
 * Server handler buildings module (header).
 ***/

#ifndef BUILDINGS_HANDLER_HPP_
#define BUILDINGS_HANDLER_HPP_

#if 0

#include <crow/http_response.h>
#include <crow/json.h>

// project namespace, server module namespace
namespace roomsched::server {

class buildings_handler {
public:
    crow::response get_all_buildings();
    crow::response get_building(int id);
    crow::response get_building_rooms(int building_id);
};

}  // namespace roomsched::server

#endif  // 0

#endif  // BUILDINGS_HANDLER_HPP_
