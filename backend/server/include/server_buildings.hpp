/***
 * Server module for buildings (header).
 ***/

#ifndef SERVER_BUILDINGS_HPP_
#define SERVER_BUILDINGS_HPP_

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

#endif  // SERVER_BUILDINGS_HPP_
