/***
 * Server handler rooms module (source).
 ***/

#include "server/handlers/room_handler.hpp"
#include "server/utils/json_utils.hpp"

namespace roomsched::server {
room_handler::room_handler(db::database_manager &db_) : db(db_) {
}

crow::response room_handler::get_all_rooms() {
    auto rooms = db.rooms().get_all_rooms();

    crow::json::wvalue resp;

    for (size_t i = 0; i < rooms.size(); i++) {
        resp[i]["id"] = rooms[i].id;
        resp[i]["room_number"] = rooms[i].room_number;
        resp[i]["building"] = rooms[i].building;
        resp[i]["floor"] = rooms[i].floor;
        resp[i]["total_area"] = rooms[i].total_area;
        resp[i]["description"] = rooms[i].description;
        resp[i]["type"] = db::convert_roomtype_to_string(rooms[i].type);
    }

    return crow::response(200, resp);
}

crow::response room_handler::get_room_by_id(int id) {
    auto current_room = db.rooms().get_room_by_id(id);

    crow::json::wvalue resp;

    resp["id"] = current_room.id;
    resp["room_number"] = current_room.room_number;
    resp["building"] = current_room.building;
    resp["floor"] = current_room.floor;
    resp["total_area"] = current_room.total_area;
    resp["description"] = current_room.description;
    resp["type"] = db::convert_roomtype_to_string(current_room.type);

    return crow::response(200, resp);
}

crow::response
room_handler::get_room_availability(const crow::request &req, int room_id) {
    auto date = req.url_params.get("date");
    auto start = req.url_params.get("start_time");
    auto end = req.url_params.get("end_time");
    if (!date || !start || !end) {
        return crow::response(400, "Missing fields in json data");
    }

    db::room_availability avail;
    avail.room_id = room_id;
    avail.date = date;
    avail.available_from = start;
    avail.available_to = end;

    // TODO: make it bool??
    db.rooms().set_availability(avail);

    crow::json::wvalue resp;
    resp["status"] = "success";
    resp["message"] = "New availability set";

    std::cout << "[ROOM_AVAILAB]: set availability success: " << std::endl;
    return crow::response(200, resp);
}

}  // namespace roomsched::server
