/***
 * Server handler booking module (source).
 ***/

#include "server/handlers/booking_handler.hpp"
#include <string>
#include "db_manager.hpp"
#include "server/utils/error_codes.hpp"
#include "server/utils/json_utils.hpp"
#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>

bool is_past_time(const std::string& date_str, const std::string& time_str) {
    std::string full_datetime = date_str + " " + time_str; // "YYYY-MM-DD HH:MM"
    std::tm tm_struct = {};
    std::istringstream ss(full_datetime);
    ss >> std::get_time(&tm_struct, "%Y-%m-%d %H:%M");
    if (ss.fail()) {
        return false;
    }
    std::time_t booking_time_t = std::mktime(&tm_struct);
    auto now = std::chrono::system_clock::now();
    std::time_t now_t = std::chrono::system_clock::to_time_t(now);
    return booking_time_t < (now_t + 60);
}

namespace roomsched::server {

bookings_handler::bookings_handler(db::database_manager &db_) : db(db_) {
}

void bookings_handler::start_cleanup() {
    cleanup_thread_ = std::thread(&bookings_handler::run_cleanup_worker, this);
}

bookings_handler::~bookings_handler() {
    stop_worker_ = true;
    if (cleanup_thread_.joinable()) {
        cleanup_thread_.join();
    }
}

void bookings_handler::run_cleanup_worker() {
    while (!stop_worker_) {
        for (int i = 0; i < 300 && !stop_worker_; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        if (stop_worker_) break;
        try {
            std::lock_guard<std::mutex> lock(db_mutex_);
            db.bookings().delete_past_bookings(); 
        } catch (const std::exception& e) {
            std::cerr << "[CLEANUP WORKER ERROR]: " << e.what() << std::endl;
        }
    }
}

crow::response bookings_handler::create_booking(const crow::request &req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return json_utils::error_response(
            "Invalid JSON",
            400,
            error_codes::kInvalidJson
        );
    }

    if (!json_utils::validate_fields(
            json,
            {"room_id", "user_id", "booking_date", "start_time", "end_time"}
        )) {
        return json_utils::error_response(
            "Missing fields in json data",
            400,
            error_codes::kMissingFields
        );
    }

    int room_id = json["room_id"].i();
    int user_id = json["user_id"].i();
    std::string date = json["booking_date"].s();
    std::string start = json["start_time"].s();
    std::string end = json["end_time"].s();

    if (is_past_time(date, start)) {
        return json_utils::error_response(
            "Cannot create booking for past time",
            400,
            error_codes::kInvalidTimeRange
        );
    }

    try {
        std::optional<roomsched::db::booking> created;
        {
            std::lock_guard<std::mutex> lock(db_mutex_);
            created = db.bookings().create_booking(room_id, user_id, date, start, end);
        }
        
        if (!created) {
            std::cerr << "[BOOKING]: invalid time range on " << date
                      << " from '" << start << "' to '" << end << "' "
                      << std::endl;
            return json_utils::error_response(
                "Invalid time range",
                422,
                error_codes::kInvalidTimeRange
            );
        }

        crow::json::wvalue resp;
        resp["status"] = "success";
        resp["message"] = "Booking created!";

        std::cout << "[BOOKING]: booking creation success: " << std::endl;
        return crow::response(200, resp);
    } catch (const std::exception& e) {
        std::string err_msg = e.what();
        if (err_msg == "ROOM_ALREADY_BOOKED") {
            return json_utils::error_response(
                "Room is already booked for this time",
                409,
                error_codes::kBookingConflict
            );
        }
        return json_utils::error_response(
            "Unexpected booking error",
            500,
            error_codes::kInternalError
        );
    }
}

crow::response bookings_handler::cancel_booking(int booking_id) {
    std::cout << "[DEBUG] Попытка отмены. Получен ID: " << booking_id << std::endl;
    std::lock_guard<std::mutex> lock(db_mutex_);
    
    bool success = db.bookings_service().cancel_booking(booking_id);
    if (!success) {
        return json_utils::error_response(
            "Booking not found",
            404,
            error_codes::kBookingNotFound
        );
    }

    crow::json::wvalue response_json;
    response_json["status"] = "success";
    response_json["message"] = "Booking cancelled";
    
    return crow::response(200, response_json);
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
    std::vector<roomsched::db::booking> bookings;
        {
        std::lock_guard<std::mutex> lock(db_mutex_);
        bookings = db.bookings().get_user_bookings(user_id);
    };

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
