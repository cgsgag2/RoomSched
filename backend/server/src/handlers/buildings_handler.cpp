/***
 * Server handler buildings module (source).
 ***/

#include "server/handlers/buildings_handler.hpp"

namespace roomsched::server {

buildings_handler::buildings_handler(db::database_manager &db_) : db(db_) {
}

crow::response buildings_handler::get_all_buildings() {
    auto buildings = db.buildings().get_all_buildings();
    crow::json::wvalue resp;

    for (size_t i = 0; i < buildings.size(); i++) {
        resp[i]["id"] = buildings[i].id;
        resp[i]["name"] = buildings[i].name;
        resp[i]["address"] = buildings[i].address;
    }

    return crow::response(200, resp);
}

}  // namespace roomsched::server

