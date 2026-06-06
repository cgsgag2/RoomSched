/***
 * Server handler rooms module (source).
 ***/

#include "server/handlers/room_handler.hpp"
#include "server/utils/error_codes.hpp"
#include "server/utils/json_utils.hpp"

namespace roomsched::server {
namespace {
std::optional<bool> parse_bool(const char *value) {
    if (!value) {
        return std::nullopt;
    }
    std::string str(value);
    if (str == "1" || str == "true" || str == "TRUE") {
        return true;
    }
    if (str == "0" || str == "false" || str == "FALSE") {
        return false;
    }
    return std::nullopt;
}

std::optional<int> parse_int(const char *value) {
    if (!value) {
        return std::nullopt;
    }
    try {
        return std::stoi(value);
    } catch (const std::exception &) {
        return std::nullopt;
    }
}

std::optional<std::string> parse_string(const char *value) {
    if (!value) {
        return std::nullopt;
    }
    return std::string(value);
}

int get_room_capacity(const db::room &room) {
    if (room.is_lecture_room()) {
        return room.capacity.value_or(0);
    }
    if (room.is_coworking_room()) {
        return room.total_capacity.value_or(0);
    }
    if (room.is_office_room()) {
        return room.number_of_chairs.value_or(0);
    }
    return 0;
}
}  // namespace

room_handler::room_handler(db::database_manager &db_) : db(db_) {
}

crow::response room_handler::get_all_rooms(const crow::request &req) {
    db::room_filter filter;
    filter.building = parse_string(req.url_params.get("building"));

    if (auto type = req.url_params.get("type")) {
        try {
            filter.type = db::convert_string_to_roomtype(type);
        } catch (const std::exception &) {
            return json_utils::error_response(
                "Unknown room type",
                400,
                error_codes::kBadRequest
            );
        }
    }

    filter.capacity_min = parse_int(req.url_params.get("capacity_min"));
    filter.capacity_max = parse_int(req.url_params.get("capacity_max"));
    filter.has_projector = parse_bool(req.url_params.get("has_projector"));
    filter.has_whiteboard = parse_bool(req.url_params.get("has_whiteboard"));
    filter.has_wifi = parse_bool(req.url_params.get("has_wifi"));
    filter.has_printers = parse_bool(req.url_params.get("has_printers"));
    filter.has_phone = parse_bool(req.url_params.get("has_phone"));

    filter.date = parse_string(req.url_params.get("date"));
    filter.start_time = parse_string(req.url_params.get("start_time"));
    filter.end_time = parse_string(req.url_params.get("end_time"));

    auto rooms = db.rooms().find_rooms(filter);

    crow::json::wvalue resp;

    for (size_t i = 0; i < rooms.size(); i++) {
        resp[i]["id"] = rooms[i].id;
        resp[i]["room_number"] = rooms[i].room_number;
        resp[i]["building"] = rooms[i].building;
        resp[i]["floor"] = rooms[i].floor;
        resp[i]["total_area"] = rooms[i].total_area;
        resp[i]["description"] = rooms[i].description;
        resp[i]["type"] = db::convert_roomtype_to_string(rooms[i].type);
        resp[i]["capacity"] = get_room_capacity(rooms[i]);

        if (rooms[i].has_projector) {
            resp[i]["has_projector"] = rooms[i].has_projector.value();
        }
        if (rooms[i].has_whiteboard) {
            resp[i]["has_whiteboard"] = rooms[i].has_whiteboard.value();
        }
        if (rooms[i].capacity) {
            resp[i]["lecture_capacity"] = rooms[i].capacity.value();
        }
        if (rooms[i].total_capacity) {
            resp[i]["coworking_capacity"] =
                rooms[i].total_capacity.value();
        }
        if (rooms[i].has_wifi) {
            resp[i]["has_wifi"] = rooms[i].has_wifi.value();
        }
        if (rooms[i].has_printers) {
            resp[i]["has_printers"] = rooms[i].has_printers.value();
        }
        if (rooms[i].number_of_chairs) {
            resp[i]["office_chairs"] = rooms[i].number_of_chairs.value();
        }
        if (rooms[i].has_phone) {
            resp[i]["has_phone"] = rooms[i].has_phone.value();
        }
    }

    return crow::response(200, resp);
}

crow::response room_handler::get_room_by_id(int id) {
    auto current_room = db.rooms().get_room_by_id(id);
    if (!current_room) {
        return json_utils::error_response(
            "Room not found", 404, error_codes::kRoomNotFound
        );
    }

    crow::json::wvalue resp;

    resp["id"] = current_room->id;
    resp["room_number"] = current_room->room_number;
    resp["building"] = current_room->building;
    resp["floor"] = current_room->floor;
    resp["total_area"] = current_room->total_area;
    resp["description"] = current_room->description;
    resp["type"] = db::convert_roomtype_to_string(current_room->type);

    return crow::response(200, resp);
}

crow::response
room_handler::get_room_availability(const crow::request &req, int room_id) {
    auto date = req.url_params.get("date");
    auto start = req.url_params.get("start_time");
    auto end = req.url_params.get("end_time");
    if (!date || !start || !end) {
        return json_utils::error_response(
            "Missing fields in query params", 400, error_codes::kMissingFields
        );
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
