/***
 * Database booking repository module (source).
 ***/

#include "repositories/booking_repository.hpp"
#include <exception>
#include <iostream>

roomsched::db::booking_repository::booking_repository(database &db_) : db(db_) {
}

void roomsched::db::booking_repository::create_booking(
    const booking &new_booking
) {
    try {
        db.execute(
            "INSERT INTO room_booking(room_id, user_id, booking_date, "
            "start_time, end_time) VALUES($1, $2, $3, $4, $5, $6) RETURNING "
            "id",
            new_booking.room_id, new_booking.user_id, new_booking.start_time,
            new_booking.end_time
        );
    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in create_user]: " << e.what() << std::endl
                  << "Query: " << e.query() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in create_user]: " << e.what() << std::endl;
    }
}
