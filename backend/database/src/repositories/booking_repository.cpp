/***
 * Database booking repository module (source).
 ***/

#include <exception>
#include <iostream>
// Local libs
#include "repositories/booking_repository.hpp"

namespace roomsched::db {

booking_repository::booking_repository(database &db_) : db(db_) {
}

void booking_repository::create_booking(const booking &new_booking) {
    try {
        db.execute(
            "INSERT INTO room_booking(room_id, user_id, booking_date, "
            "start_time, end_time) VALUES($1, $2, $3, $4, $5) RETURNING "
            "id",
            new_booking.room_id, new_booking.user_id, new_booking.date,
            new_booking.start_time, new_booking.end_time
        );
    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in create_booking]: " << e.what() << std::endl
                  << "Query: " << e.query() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in create_booking]: " << e.what()
                  << std::endl;
    }
}

void roomsched::db::booking_repository::delete_booking(int id) {
    try {
        db.execute("DELETE FROM room_booking WHERE id = $1", id);
    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in delete_booking]: " << e.what() << std::endl
                  << "Query: " << e.query() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in delete_booking]: " << e.what()
                  << std::endl;
    }
}

std::optional<booking> booking_repository::get_booking_by_id(int id) {
    try {
        auto result = db.query(
            "SELECT id, room_id, user_id, booking_date, start_time, end_time, "
            "created_at, status FROM room_booking WHERE id = $1",
            id
        );

        if (result.empty()) {
            throw std::runtime_error("Booking not found");
        }

        const auto &row = result[0];
        booking b;

        b.id = row["id"].as<int>();
        b.room_id = row["room_id"].as<int>();
        b.user_id = row["user_id"].as<int>();

        b.date = row["booking_date"].as<std::string>();
        b.start_time = row["start_time"].as<std::string>();
        b.end_time = row["end_time"].as<std::string>();
        b.created_at = row["created_at"].as<std::string>();
        b.status =
            convert_string_to_booking_status(row["status"].as<std::string>());

        return b;
    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in get_booking_by_id]: " << e.what()
                  << std::endl
                  << "Query: " << e.query() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in get_booking_by_id]: " << e.what()
                  << std::endl;
    }
    return std::nullopt;
}

std::vector<booking> booking_repository::get_all_bookings() {
    std::vector<booking> bookings;

    try {
        auto result = db.query(
            "SELECT id, room_id, user_id, booking_date, start_time, end_time, "
            "created_at, status FROM room_booking ORDER BY booking_date, "
            "start_time"
        );

        for (const auto &row : result) {
            booking b;

            b.id = row["id"].as<int>();
            b.room_id = row["room_id"].as<int>();
            b.user_id = row["user_id"].as<int>();
            b.date = row["booking_date"].as<std::string>();
            b.start_time = row["start_time"].as<std::string>();
            b.end_time = row["end_time"].as<std::string>();
            b.created_at = row["created_at"].as<std::string>();
            b.status =
                convert_string_to_booking_status(row["status"].as<std::string>()
                );

            bookings.push_back(b);
        }

    } catch (const std::exception &e) {
        std::cerr << "[DB ERROR in get_all_bookings]: " << e.what()
                  << std::endl;
    }

    return bookings;
}

std::vector<booking> booking_repository::get_bookings_by_user(int user_id) {
    std::vector<booking> bookings;

    try {
        auto result = db.query(
            "SELECT id, room_id, user_id, booking_date, start_time, end_time, "
            "created_at, status FROM room_booking WHERE user_id = $1 ORDER BY "
            "booking_date, start_time",
            user_id
        );

        for (const auto &row : result) {
            booking b;

            b.id = row["id"].as<int>();
            b.room_id = row["room_id"].as<int>();
            b.user_id = row["user_id"].as<int>();
            b.date = row["booking_date"].as<std::string>();
            b.start_time = row["start_time"].as<std::string>();
            b.end_time = row["end_time"].as<std::string>();
            b.created_at = row["created_at"].as<std::string>();
            b.status =
                convert_string_to_booking_status(row["status"].as<std::string>()
                );

            bookings.push_back(b);
        }

    } catch (const std::exception &e) {
        std::cerr << "[DB ERROR in get_bookings_by_user]: " << e.what()
                  << std::endl;
    }

    return bookings;
}

std::vector<booking> booking_repository::get_bookings_by_room(int room_id) {
    std::vector<booking> bookings;

    try {
        auto result = db.query(
            "SELECT id, room_id, user_id, booking_date, start_time, end_time, "
            "created_at, status FROM room_booking WHERE room_id = $1 ORDER BY "
            "booking_date, start_time",
            room_id
        );

        for (const auto &row : result) {
            booking b;

            b.id = row["id"].as<int>();
            b.room_id = row["room_id"].as<int>();
            b.user_id = row["user_id"].as<int>();
            b.date = row["booking_date"].as<std::string>();
            b.start_time = row["start_time"].as<std::string>();
            b.end_time = row["end_time"].as<std::string>();
            b.created_at = row["created_at"].as<std::string>();
            b.status =
                convert_string_to_booking_status(row["status"].as<std::string>()
                );

            bookings.push_back(b);
        }

    } catch (const std::exception &e) {
        std::cerr << "[DB ERROR in get_bookings_by_room]: " << e.what()
                  << std::endl;
    }

    return bookings;
}

bool booking_repository::update_status(int id, booking_status new_status) {
    try {
        db.execute(
            "UPDATE room_booking SET status = $1 WHERE id = $2",
            convert_booking_status_to_string(new_status), id
        );
        return true;
    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in update_status]: " << e.what() << std::endl
                  << "Query: " << e.query() << std::endl;
        return false;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in update_status]: " << e.what()
                  << std::endl;
        return false;
    }
}

bool booking_repository::is_room_already_booked(
    int room_id,
    const std::string &date,
    const std::string &start,
    const std::string &end
) {
    try {
        auto result = db.query(
            "SELECT 1 FROM room_booking WHERE room_id = $1 AND booking_date = "
            "$2 AND status = 'confirmed' AND start_time < $4 AND end_time > $3 "
            "LIMIT 1",
            room_id, date, start, end
        );

        return !result.empty();
    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in is_room_already_booked]: " << e.what()
                  << std::endl
                  << "Query: " << e.query() << std::endl;
        // NOTE: if something went wrong, we will not allow to work
        return true;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in is_room_already_booked]: " << e.what()
                  << std::endl;
        // NOTE: if something went wrong, we will not allow to work
        return true;
    }
}

}  // namespace roomsched::db
