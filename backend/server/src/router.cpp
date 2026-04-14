/***
 * Server router module (source).
 ***/

#include <crow.h>
// Local includes
#include "server/app.hpp"
#include "server/router.hpp"

void roomsched::server::setup_all_routes(
    crow::SimpleApp &app_ref,
    app &server
) {
    CROW_ROUTE(app_ref, "/")([]() { return "Hello Diana and Alex :))"; });

    CROW_ROUTE(app_ref, "/health")
    ([]() {
        crow::json::wvalue res;
        res["status"] = "OK";
        return res;
    });

    /* User module */

    CROW_ROUTE(app_ref, "/register")
        .methods("POST"_method)([&server](const crow::request &req) {
            return server.get_auth_handler().register_user(req);
        });

    CROW_ROUTE(app_ref, "/login")
        .methods("POST"_method)([&server](const crow::request &req) {
            return server.get_auth_handler().login(req);
        });

    CROW_ROUTE(app_ref, "/get_users").methods("GET"_method)([&server]() {
        return server.get_auth_handler().get_all_users();
    });

    /* Rooms and room availability module */
    CROW_ROUTE(app_ref, "/rooms").methods("GET"_method)([&server]() {
        return server.get_room_handler().get_all_rooms();
    });

    CROW_ROUTE(app_ref, "/rooms/<int>")
        .methods("GET"_method)([&server](int room_id) {
            return server.get_room_handler().get_room_by_id(room_id);
        });

    // TODO: not tested yet, after MVP
    CROW_ROUTE(app_ref, "/rooms/<int>/availability")
        .methods("GET"_method
        )([&server](const crow::request &req, int room_id) {
            return server.get_room_handler().get_room_availability(
                req, room_id
            );
        });

    /* Booking module */
    CROW_ROUTE(app_ref, "/book-room")
        .methods("POST"_method)([&server](const crow::request &req) {
            return server.get_booking_handler().create_booking(req);
        });

    CROW_ROUTE(app_ref, "/bookings").methods("GET"_method)([&server]() {
        return server.get_booking_handler().get_all_bookings();
    });

    CROW_ROUTE(app_ref, "/bookings/user/<int>")
        .methods("GET"_method)([&server](int user_id) {
            return server.get_booking_handler().get_bookings_by_user(user_id);
        });

    CROW_ROUTE(app_ref, "/bookings/rooms/<int>")
        .methods("GET"_method)([&server](int room_id) {
            return server.get_booking_handler().get_bookings_by_room(room_id);
        });

    // Not MVP
    CROW_ROUTE(app_ref, "/booking/<int>/cancel")
        .methods("POST"_method)([&server](int booking_id) {
            return server.get_booking_handler().cancel_booking(booking_id);
        });

    /* Buildings module - for future */

    // CROW_ROUTE(app, "/buildings")
    // ([&buildings]() { return buildings.get_all_buildings(); });
    // CROW_ROUTE(app, "/buildings/<int>")
    // ([&buildings](int id) { return buildings.get_building(id); });
    // CROW_ROUTE(app, "/buildings/<int>/rooms")
    // ([&buildings](int id) { return buildings.get_building_rooms(id); });
}
