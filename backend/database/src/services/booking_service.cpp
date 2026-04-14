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
    // TODO: add room availability later!
    // bool available = roomService.is_room_available(room_id, date, start,
    // end);

    // if (!available) {
    //     std::cerr << "This room is not available at " << date
    //               << " at time from " << start << " to " << end << std::endl;
    //     return std::nullopt;
    // }
    if (start >= end) {
        std::cerr << "Invalid time range" << std::endl;
        return std::nullopt;
    }

    if (booking_repo.is_room_already_booked(room_id, date, start, end)) {
        std::cerr << "Room '" << room_id << "' is already booked" << std::endl;
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
    auto cur_book = booking_repo.get_booking_by_id(booking_id);

    if (!cur_book) {
        return false;
    }

    if (cur_book->status == booking_status::CANCELLED) {
        return false;
    }

    booking_repo.update_status(booking_id, booking_status::CANCELLED);
    return true;
}

std::vector<roomsched::db::booking>
roomsched::db::booking_service::get_all_bookings() {
    return booking_repo.get_all_bookings();
}

std::vector<roomsched::db::booking>
roomsched::db::booking_service::get_user_bookings(int user_id) {
    return booking_repo.get_bookings_by_user(user_id);
}

std::vector<roomsched::db::booking>
roomsched::db::booking_service::get_room_bookings(int room_id) {
    return booking_repo.get_bookings_by_room(room_id);
}
