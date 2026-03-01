

#ifndef ROOMSCHED_SERVER_BOOKINGS_HPP
#define ROOMSCHED_SERVER_BOOKINGS_HPP
#include <crow/http_response.h>
#include <crow/json.h>
#include <string>
#include "db_manager.hpp"
// project namespace, server module namespace
namespace roomsched::server {
class bookings_handler {
    public:
    crow::response create_booking(const crow::request &req, db::database_manager &database);
    crow::response cancel_booking(int booking_id, db::database_manager& db);
    crow::response get_user_bookings(int user_id, db::database_manager& db);
    crow::response create_availability(const crow::request& req,db::database_manager& db,int room_id);



};
}
#endif  // ROOMSCHED_SERVER_BOOKINGS_HPP
