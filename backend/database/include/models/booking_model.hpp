/***
 * Database booking model module (header).
 ***/

#ifndef BOOKING_MODEL_HPP_
#define BOOKING_MODEL_HPP_

#include <ostream>
#include <string>

// project namespace, database module namespace
namespace roomsched::db {
enum class booking_status { PENDING, CONFIRMED, CANCELLED };

class booking {
public:
    int id;
    int room_id;
    int user_id;

    std::string date;
    std::string start_time;
    std::string end_time;

    std::string created_at;
    booking_status status;
};

inline booking_status convert_string_to_booking_status(const std::string &str) {
    if (str == "PENDING" || str == "pending") {
        return booking_status::PENDING;
    } else if (str == "CONFIRMED" || str == "confirmed") {
        return booking_status::CONFIRMED;
    } else if (str == "CANCELLED" || str == "cancelled") {
        return booking_status::CANCELLED;
    } else {
        throw std::runtime_error("Unknown booking status!");
    }
}

inline std::string convert_booking_status_to_string(booking_status type) {
    if (type == booking_status::PENDING) {
        return "pending";
    } else if (type == booking_status::CONFIRMED) {
        return "confirmed";
    } else if (type == booking_status::CANCELLED) {
        return "cancelled";
    } else {
        throw std::runtime_error("Unknown booking status!");
    }
}

}  // namespace roomsched::db

#endif  // BOOKING_MODEL_HPP_
