#pragma once
#include "crow/http_response.h"
#include "crow/json.h"
#include <string>

// Структура для бронирования (пока без БД)
struct Booking {
    int id;
    int room_id;
    int user_id;
    std::string start_time;
    std::string end_time;
    std::string date;
};

// API функции
crow::response get_all_bookings();
crow::response get_user_bookings(int user_id);
crow::response get_room_bookings(int room_id);
crow::response create_booking(const crow::request& req);
crow::response cancel_booking(int booking_id);
crow::response check_availability(const crow::request& req);