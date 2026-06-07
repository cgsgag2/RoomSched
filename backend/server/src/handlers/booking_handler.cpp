/***
 * Server handler booking module (source).
 ***/

#include "server/handlers/booking_handler.hpp"
#include <string>
#include "db_manager.hpp"
#include "server/utils/error_codes.hpp"
#include "server/utils/json_utils.hpp"

namespace roomsched::server {

bookings_handler::bookings_handler(db::database_manager &db_) : db(db_) {
}

crow::response bookings_handler::create_booking(const crow::request &req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return json_utils::error_response(
            "Invalid JSON", 400, error_codes::kInvalidJson
        );
    }

    if (!json_utils::validate_fields(
            json,
            {"room_id", "user_id", "booking_date", "start_time", "end_time"}
        )) {
        return json_utils::error_response(
            "Missing fields in json data", 400, error_codes::kMissingFields
        );
    }

    int room_id = json["room_id"].i();
    int user_id = json["user_id"].i();
    std::string date = json["booking_date"].s();
    std::string start = json["start_time"].s();
    std::string end = json["end_time"].s();

    try {
        auto created =
            db.bookings().create_booking(room_id, user_id, date, start, end);

        if (!created) {
            std::cerr << "[BOOKING]: invalid time range on " << date
                      << " from '" << start << "' to '" << end << "' "
                      << std::endl;
            return json_utils::error_response(
                "Invalid time range", 422, error_codes::kInvalidTimeRange
            );
        }

        auto chat_id = db.users().get_telegram_chat_id(user_id);
        if (chat_id) {
            std::string message =
                "Booking confirmed!\n\nRoom: " + std::to_string(room_id) +
                "\nDate: " + date + "\nTime: " + start + " - " + end;

            bool success = db.telegram().send_message(*chat_id, message);

            if (!success) {
                std::cerr << "[BOOKING][TELEGRAM] "
                          << "failed to send notification" << std::endl;
            }
        }

        crow::json::wvalue resp;
        resp["status"] = "success";
        resp["message"] = "Booking created!";

        std::cout << "[BOOKING]: booking creation success: " << std::endl;
        return crow::response(200, resp);
    } catch (const std::exception &e) {
        std::string err_msg = e.what();
        if (err_msg == "ROOM_ALREADY_BOOKED") {
            return json_utils::error_response(
                "Room is already booked for this time", 409,
                error_codes::kBookingConflict
            );
        }
        return json_utils::error_response(
            "Unexpected booking error", 500, error_codes::kInternalError
        );
    }
}

crow::response bookings_handler::cancel_booking(int booking_id) {
    bool success = db.bookings().cancel_booking(booking_id);
    if (!success) {
        return json_utils::error_response(
            "Booking not found", 404, error_codes::kBookingNotFound
        );
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
