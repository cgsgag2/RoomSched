/***
 * Database booking service module (source).
 ***/

#include "services/booking_service.hpp"
#include <iostream>

roomsched::db::booking_service::booking_service(
    database &db,
    room_service &room_service_ref
)
    : booking_repo(db), roomService(room_service_ref) {
}

std::optional<roomsched::db::booking>
roomsched::db::booking_service::create_booking(
    int room_id,
    int user_id,
    const std::string &date,
    const std::string &start,
    const std::string &end
) {
    bool available = roomService.is_room_available(room_id, date, start, end);

    if (!available) {
        std::cerr << "This room is not available at " << date
                  << " at time from " << start << " to " << end << std::endl;
        return std::nullopt;
    }

    booking b;

    b.room_id = room_id;
    b.user_id = user_id;
    b.date = date;
    b.start_time = start;
    b.end_time = end;
    b.status = db::booking_status::CONFIRMED;

    booking_repo.create_booking(b);

    return b;
}

bool roomsched::db::booking_service::cancel_booking(int booking_id) {
#if 0
    auto booking = booking_repo.find_by_id(booking_id);

    if (!booking) {
        return false;
    }

    booking->status = db::booking_status::CANCELLED;

    booking_repo.update(*booking);
#endif  // 0
    return true;
}

std::vector<roomsched::db::booking>
roomsched::db::booking_service::get_user_bookings(int user_id) {
    // return booking_repo.find_by_user(user_id);
    return {};
}

std::vector<roomsched::db::booking>
roomsched::db::booking_service::get_room_bookings(int room_id) {
    // return booking_repo.find_by_room(room_id);
    return {};
}
