#include "server_bookings.hpp"
#include <string>
#include "db_manager.hpp"
#include "server_bookings.hpp"
#include "server_json_utils.hpp"

namespace roomsched::server {
crow::response roomsched::server::bookings_handler::create_availability(
    const crow::request &req,
    db::database_manager &db,
    int room_id
) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return json_utils::error_response("Invalid JSON format");
    }
    const std::vector<std::string> required = {
        "date", "start_time", "end_time"};
    if (!json_utils::validate_fields(json, required)) {
        return json_utils::error_response("Missing required fields");
    }
    std::string date = json["date"].s();
    std::string start = json["start_time"].s();
    std::string end = json["end_time"].s();

    // DB
    //bool available = db.is_room_available(room_id, date, start, end);
    bool available = true;

    crow::json::wvalue data;
    data["room_id"] = room_id;
    data["date"] = date;
    data["start_time"] = start;
    data["end_time"] = end;
    data["available"] = available;

    if (available) {
        return json_utils::success_response("Room is available", data);
    } else {
        return json_utils::success_response("Room is not available", data);
    }
}

crow::response roomsched::server::bookings_handler::create_booking(
    const crow::request &req,
    db::database_manager &database
) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return json_utils::error_response("Invalid JSON format");
    }
    const std::vector<std::string> required = {
        "room_id", "user_id", "date", "start_time", "end_time"};
    if (!json_utils::validate_fields(json, required)) {
        return json_utils::error_response("Missing required fields");
    }
    int room_id = json["room_id"].i();
    int user_id = json["user_id"].i();
    std::string date = json["date"].s();
    std::string start = json["start_time"].s();
    std::string end = json["end_time"].s();

    // is room available
    //if (!db.is_room_available(room_id, date, start, end)) {
    //    return json_utils::error_response(
    //       "Room already booked for this time", 409
    //   );
    //}

    //DB
    //int booking_id = db.create_booking(room_id, user_id, date, start, end);
    // return id
    int booking_id = 55;

    crow::json::wvalue data;
    data["booking_id"] = booking_id;
    data["room_id"] = room_id;
    data["date"] = date;
    data["start_time"] = start;
    data["end_time"] = end;
    return json_utils::success_response("Booking created", data);
}

crow::response roomsched::server::bookings_handler::cancel_booking(
    int booking_id,
    db::database_manager &db
) {
    // проверка на available не нужна, это другое
    //DB
    //bool success = db.cancel_booking(booking_id);
    bool success = true;

    if (!success) {
        return json_utils::error_response("Booking not found", 404);
    }

    crow::json::wvalue data;
    data["booking_id"] = booking_id;

    return json_utils::success_response("Booking cancelled", data);
}

crow::response roomsched::server::bookings_handler::get_user_bookings(
    int user_id,
    db::database_manager &db
) {
    //DB
    //std::vector<db::booking> bookings = db.get_user_bookings(user_id);
    //заглушка
    struct TempBooking {
        int id;
        int room_id;
        std::string date;
        std::string start_time;
        std::string end_time;
    };
    std::vector<TempBooking> bookings = {
        {1, 101, "2026-03-15", "10:00", "12:00"},
        {2, 102, "2026-03-16", "14:00", "16:00"}
    };

    crow::json::wvalue::list bookings_list;
    for (const auto &b : bookings) {
        crow::json::wvalue room_json;
        room_json["id"] = b.id;
        room_json["room_id"] = b.room_id;
        room_json["date"] = b.date;
        room_json["start_time"] = b.start_time;
        room_json["end_time"] = b.end_time;
        bookings_list.push_back(std::move(room_json));
    }
    crow::json::wvalue data;
    data["user_id"] = user_id;
    data["bookings"] = std::move(bookings_list);
    data["count"] = bookings.size();

    return json_utils::success_response("User bookings retrieved", data);
}

}  // namespace roomsched::server