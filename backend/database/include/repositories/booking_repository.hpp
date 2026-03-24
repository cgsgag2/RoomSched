/***
 * Database booking repository module (header).
 ***/

#ifndef BOOKING_REPOSITORY_HPP_
#define BOOKING_REPOSITORY_HPP_

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
};
}  // namespace roomsched::db

#endif  // BOOKING_REPOSITORY_HPP_
