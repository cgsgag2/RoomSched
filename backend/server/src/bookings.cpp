#include "bookings.h"
#include "json_utils.h"
#include <vector>
#include <algorithm>

// Временное хранилище (пока нет БД)
static std::vector<Booking> bookings = {
    {1, 101, 1, "10:00", "12:00", "2026-02-14"},
    {2, 101, 2, "14:00", "16:00", "2026-02-14"},
    {3, 102, 1, "11:00", "13:00", "2026-02-15"}
};

crow::response get_all_bookings() {
    crow::json::wvalue::list bookings_list;
    
    for (const auto& b : bookings) {
        crow::json::wvalue booking;
        booking["id"] = b.id;
        booking["room_id"] = b.room_id;
        booking["user_id"] = b.user_id;
        booking["date"] = b.date;
        booking["start_time"] = b.start_time;
        booking["end_time"] = b.end_time;
        bookings_list.push_back(std::move(booking));
    }
    
    crow::json::wvalue data;
    data["bookings"] = std::move(bookings_list);
    data["count"] = bookings.size();
    
    return json_utils::success_response("Bookings retrieved", data);
}

crow::response get_user_bookings(int user_id) {
    crow::json::wvalue::list user_bookings;
    
    for (const auto& b : bookings) {
        if (b.user_id == user_id) {
            crow::json::wvalue booking;
            booking["id"] = b.id;
            booking["room_id"] = b.room_id;
            booking["date"] = b.date;
            booking["start_time"] = b.start_time;
            booking["end_time"] = b.end_time;
            user_bookings.push_back(std::move(booking));
        }
    }
    
    crow::json::wvalue data;
    data["user_id"] = user_id;
    data["bookings"] = std::move(user_bookings);
    data["count"] = user_bookings.size();
    
    return json_utils::success_response("User bookings retrieved", data);
}

crow::response get_room_bookings(int room_id) {
    crow::json::wvalue::list room_bookings;
    
    for (const auto& b : bookings) {
        if (b.room_id == room_id) {
            crow::json::wvalue booking;
            booking["id"] = b.id;
            booking["user_id"] = b.user_id;
            booking["date"] = b.date;
            booking["start_time"] = b.start_time;
            booking["end_time"] = b.end_time;
            room_bookings.push_back(std::move(booking));
        }
    }
    
    crow::json::wvalue data;
    data["room_id"] = room_id;
    data["bookings"] = std::move(room_bookings);
    data["count"] = room_bookings.size();
    
    return json_utils::success_response("Room bookings retrieved", data);
}

crow::response create_booking(const crow::request& req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return json_utils::error_response("Invalid JSON format");
    }
    
    std::vector<std::string> required = {"room_id", "user_id", "date", "start_time", "end_time"};
    if (!json_utils::validate_fields(json, required)) {
        return json_utils::error_response("Missing required fields");
    }
    
    int room_id = json["room_id"].i();
    int user_id = json["user_id"].i();
    std::string date = json["date"].s();
    std::string start = json["start_time"].s();
    std::string end = json["end_time"].s();
    
    // Проверка на конфликт бронирований
    for (const auto& b : bookings) {
        if (b.room_id == room_id && b.date == date) {
            // Проверка пересечения времени
            if (!(end <= b.start_time || start >= b.end_time)) {
                return json_utils::error_response("Room already booked for this time", 409);
            }
        }
    }
    
    // Создаем новое бронирование
    int new_id = bookings.size() + 1;
    Booking new_booking = {new_id, room_id, user_id, start, end, date};
    bookings.push_back(new_booking);
    
    crow::json::wvalue data;
    data["booking_id"] = new_id;
    data["room_id"] = room_id;
    data["date"] = date;
    data["start_time"] = start;
    data["end_time"] = end;
    
    return json_utils::success_response("Booking created", data);
}

crow::response cancel_booking(int booking_id) {
    auto it = std::find_if(bookings.begin(), bookings.end(), 
        [booking_id](const Booking& b) { return b.id == booking_id; });
    
    if (it == bookings.end()) {
        return json_utils::error_response("Booking not found", 404);
    }
    
    bookings.erase(it);
    
    crow::json::wvalue data;
    data["booking_id"] = booking_id;
    
    return json_utils::success_response("Booking cancelled", data);
}

crow::response check_availability(const crow::request& req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return json_utils::error_response("Invalid JSON format");
    }
    
    std::vector<std::string> required = {"room_id", "date", "start_time", "end_time"};
    if (!json_utils::validate_fields(json, required)) {
        return json_utils::error_response("Missing required fields");
    }
    
    int room_id = json["room_id"].i();
    std::string date = json["date"].s();
    std::string start = json["start_time"].s();
    std::string end = json["end_time"].s();
    
    bool available = true;
    std::string conflicting_booking;
    
    for (const auto& b : bookings) {
        if (b.room_id == room_id && b.date == date) {
            if (!(end <= b.start_time || start >= b.end_time)) {
                available = false;
                conflicting_booking = "Booked from " + b.start_time + " to " + b.end_time;
                break;
            }
        }
    }
    
    crow::json::wvalue data;
    data["room_id"] = room_id;
    data["date"] = date;
    data["start_time"] = start;
    data["end_time"] = end;
    data["available"] = available;
    if (!available) {
        data["conflict"] = conflicting_booking;
    }
    
    return json_utils::success_response(available ? "Room available" : "Room not available", data);
}