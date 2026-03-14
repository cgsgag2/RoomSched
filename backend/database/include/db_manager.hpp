/***
 * Database manager module (header).
 ***/

#ifndef DB_MANAGER_HPP_
#define DB_MANAGER_HPP_

#include "core/db_core.hpp"
#include "services/booking_service.hpp"
#include "services/room_service.hpp"
#include "services/user_service.hpp"

// project namespace, database module namespace
namespace roomsched::db {
class database_manager {
private:
    database db;
    user_service userService;
    room_service roomService;
    /* TODO... */

public:
    database_manager(const db_config &config);

    user_service &users();
    room_service &rooms();
};
}  // namespace roomsched::db

#endif  // DB_MANAGER_HPP_
