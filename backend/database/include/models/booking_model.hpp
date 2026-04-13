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

}  // namespace roomsched::db

#endif  // BOOKING_MODEL_HPP_
