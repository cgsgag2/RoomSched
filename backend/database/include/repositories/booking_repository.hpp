/***
 * Database booking repository module (header).
 ***/

#ifndef BOOKING_REPOSITORY_HPP_
#define BOOKING_REPOSITORY_HPP_

#include <optional>
#include "../core/db_core.hpp"
#include "../models/booking_model.hpp"

// project namespace, database module namespace
namespace roomsched::db {
class booking_repository {
private:
    database &db;

public:
    booking_repository(database &db_);

    void create_booking(const booking &new_booking);
    void delete_booking(int id);

    std::optional<booking> get_booking_by_id(int id);
    std::vector<booking> get_all_bookings();
    std::vector<booking> get_bookings_by_user(int user_id);
    std::vector<booking> get_bookings_by_room(int room_id);

    bool update_status(int id, booking_status new_status);

    bool is_room_already_booked(
        int room_id,
        const std::string &date,
        const std::string &start,
        const std::string &end
    );
};
}  // namespace roomsched::db

#endif  // BOOKING_REPOSITORY_HPP_
