/***
 * Database manager module (source).
 ***/

#include "db_manager.hpp"

roomsched::db::database_manager::database_manager(const db_config &config)
    : db(config), userService(db), roomService(db) {
    db.connect();
}

roomsched::db::user_service &roomsched::db::database_manager::users() {
    return userService;
}

roomsched::db::room_service &roomsched::db::database_manager::rooms() {
    return roomService;
}
