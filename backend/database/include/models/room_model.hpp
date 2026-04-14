/***
 * Database rooms model module (header).
 ***/

#ifndef ROOM_MODEL_HPP_
#define ROOM_MODEL_HPP_

#include <optional>
#include <ostream>
#include <string>

// project namespace, database module namespace
namespace roomsched::db {
enum class room_type {
    LECTURE,
    COWORKING,
    PRIVATE_OFFICE,
};

inline room_type convert_string_to_roomtype(const std::string &str) {
    if (str == "LECTURE" || str == "lecture") {
        return room_type::LECTURE;
    } else if (str == "COWORKING" || str == "coworking") {
        return room_type::COWORKING;
    } else if (str == "PRIVATE_OFFICE" || str == "private_office") {
        return room_type::PRIVATE_OFFICE;
    } else {
        throw std::runtime_error("Unknown room type!");
    }
}

inline std::string convert_roomtype_to_string(room_type type) {
    if (type == room_type::LECTURE) {
        return "lecture";
    } else if (type == room_type::COWORKING) {
        return "coworking";
    } else if (type == room_type::PRIVATE_OFFICE) {
        return "private_office";
    } else {
        throw std::runtime_error("Unknown room type!");
    }
}

class room {
public:
    // Common fields
    int id;
    std::string room_number;
    std::string building;
    int floor;
    double total_area;
    std::string description;

    room_type type;

    // Lecture room
    std::optional<bool> has_projector;
    std::optional<bool> has_whiteboard;
    std::optional<int> capacity;

    // Coworking space
    std::optional<int> total_capacity;
    std::optional<bool> has_wifi;
    std::optional<bool> has_printers;

    // Private office
    std::optional<int> number_of_chairs;
    std::optional<bool> has_phone;

    bool is_lecture_room() const {
        return type == room_type::LECTURE;
    }

    bool is_coworking_room() const {
        return type == room_type::COWORKING;
    }

    bool is_office_room() const {
        return type == room_type::PRIVATE_OFFICE;
    }
};

class room_availability {
public:
    int room_id;
    std::string date = "2026-03-15";  // TODO: Will be not using now, think
                                      // about it: date vs week day
    bool is_available = true;
    std::string available_from = "09:00";
    std::string available_to = "18:00";
    std::string booking_ref = "Always ready";
};

}  // namespace roomsched::db

#endif  // ROOM_MODEL_HPP_
