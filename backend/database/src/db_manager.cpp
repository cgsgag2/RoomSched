/***
 * Database manager module (source).
 ***/

#include "db_manager.hpp"

roomsched::db::database_manager::database_manager(const db_config &config)
    : db(config),
      userService(db),
      buildingService(db),
      roomService(db),
      bookingService(db),
      telegramService(db) {
    db.connect();
}

roomsched::db::user_service &roomsched::db::database_manager::users() {
    return userService;
}

roomsched::db::building_service &roomsched::db::database_manager::buildings() {
    return buildingService;
}

roomsched::db::room_service &roomsched::db::database_manager::rooms() {
    return roomService;
}

roomsched::db::booking_service &roomsched::db::database_manager::bookings() {
    return bookingService;
}

roomsched::db::telegram_service &roomsched::db::database_manager::telegram() {
    return telegramService;
}
