/***
 * Server handler booking module (source).
 ***/

#include "server/handlers/booking_handler.hpp"
#include <string>
#include "db_manager.hpp"
#include "server/utils/json_utils.hpp"

namespace roomsched::server {

bookings_handler::bookings_handler(db::database_manager &db_) : db(db_) {
}

crow::response bookings_handler::create_booking(const crow::request &req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return crow::response(400, "Invalid JSON");
    }

    if (!json.has("room_id") || !json.has("user_id") ||
        !json.has("booking_date") || !json.has("start_time") ||
        !json.has("end_time")) {
        return crow::response(400, "Missing fields in json data");
    }

    int room_id = json["room_id"].i();
    int user_id = json["user_id"].i();
    std::string date = json["booking_date"].s();
    std::string start = json["start_time"].s();
    std::string end = json["end_time"].s();

    try {
        auto is_available =
            db.bookings().create_booking(room_id, user_id, date, start, end);

        if (!is_available) {
            std::cerr << "[BOOKING]: failed to create booking on " << date
                    << " from '" << start << "' to '" << end << "' " << std::endl;
            return crow::response(409, "Some problems in creating new booking");
        }

        crow::json::wvalue resp;
        resp["status"] = "success";
        resp["message"] = "Booking created!";

        std::cout << "[BOOKING]: booking creation success: " << std::endl;
        return crow::response(200, resp);
    } catch (const std::exception& e) {
        std::string err_msg = e.what();
        if (err_msg == "ROOM_ALREADY_BOOKED") {
            return crow::response(409, "Room is already booked for this time");
        }
        return crow::response(400, err_msg);
    }
}

crow::response bookings_handler::cancel_booking(int booking_id) {
    bool success = db.bookings().cancel_booking(booking_id);
    if (!success) {
        return crow::response(404, "Booking not found");
    }

    return crow::response(200, "Booking cancelled");
}

crow::response bookings_handler::get_all_bookings() {
    auto bookings = db.bookings().get_all_bookings();

    crow::json::wvalue resp;

    for (size_t i = 0; i < bookings.size(); i++) {
        resp[i]["id"] = bookings[i].id;
        resp[i]["room_id"] = bookings[i].room_id;
        resp[i]["user_id"] = bookings[i].user_id;
        resp[i]["booking_date"] = bookings[i].date;
        resp[i]["start_time"] = bookings[i].start_time;
        resp[i]["end_time"] = bookings[i].end_time;
        resp[i]["created_at"] = bookings[i].created_at;
        resp[i]["status"] =
            db::convert_booking_status_to_string(bookings[i].status);
    }

    return crow::response(200, resp);
}

crow::response bookings_handler::get_bookings_by_user(int user_id) {
    auto bookings = db.bookings().get_user_bookings(user_id);

    crow::json::wvalue resp;

    for (size_t i = 0; i < bookings.size(); i++) {
        resp[i]["id"] = bookings[i].id;
        resp[i]["room_id"] = bookings[i].room_id;
        resp[i]["user_id"] = bookings[i].user_id;
        resp[i]["booking_date"] = bookings[i].date;
        resp[i]["start_time"] = bookings[i].start_time;
        resp[i]["end_time"] = bookings[i].end_time;
        resp[i]["created_at"] = bookings[i].created_at;
        resp[i]["status"] =
            db::convert_booking_status_to_string(bookings[i].status);
    }

    return crow::response(200, resp);
}

crow::response bookings_handler::get_bookings_by_room(int room_id) {
    auto bookings = db.bookings().get_room_bookings(room_id);

    crow::json::wvalue resp;

    for (size_t i = 0; i < bookings.size(); i++) {
        resp[i]["id"] = bookings[i].id;
        resp[i]["room_id"] = bookings[i].room_id;
        resp[i]["user_id"] = bookings[i].user_id;
        resp[i]["booking_date"] = bookings[i].date;
        resp[i]["start_time"] = bookings[i].start_time;
        resp[i]["end_time"] = bookings[i].end_time;
        resp[i]["created_at"] = bookings[i].created_at;
        resp[i]["status"] =
            db::convert_booking_status_to_string(bookings[i].status);
    }

    return crow::response(200, resp);
}

}  // namespace roomsched::server
