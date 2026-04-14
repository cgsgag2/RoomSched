/***
 * Database booking service module (header).
 ***/

#ifndef BOOKING_SERVICE_HPP_
#define BOOKING_SERVICE_HPP_

#include "../repositories/booking_repository.hpp"
#include "room_service.hpp"

// project namespace, database module namespace
namespace roomsched::db {
class booking_service {
private:
    booking_repository booking_repo;
    room_service &roomService;

public:
    booking_service(database &db, room_service &room_service_ref);

    std::optional<booking> create_booking(
        int room_id,
        int user_id,
        const std::string &date,
        const std::string &start,
        const std::string &end
    );

    bool cancel_booking(int booking_id);

    std::vector<booking> get_all_bookings();
    std::vector<booking> get_user_bookings(int user_id);
    std::vector<booking> get_room_bookings(int room_id);
};

}  // namespace roomsched::db

#endif  // BOOKING_SERVICE_HPP_
