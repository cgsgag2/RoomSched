#ifndef BUILDINGS_HPP
#define BUILDINGS_HPP
#include "crow/http_response.h"
#include "crow/json.h"

namespace roomsched {

class BuildingsHandler {
public:
    crow::response get_all_buildings();
    crow::response get_building(int id);
    crow::response get_building_rooms(int building_id);
};

} // namespace roomsched
#endif