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
            return server.get_auth().register_user(req);
        });

    CROW_ROUTE(app_ref, "/login")
        .methods("POST"_method)([&server](const crow::request &req) {
            return server.get_auth().login(req);
        });

    CROW_ROUTE(app_ref, "/get_users").methods("GET"_method)([&server]() {
        return server.get_auth().get_all_users();
    });

    /* Booking module */
    // CROW_ROUTE(app_ref, "/bookings")
    //     .methods("POST"_method
    //     )([&server](const crow::request &req) {
    //         return bookings.create_booking(req, db_manager);
    //     });
    // CROW_ROUTE(app_ref, "/bookings/user/<int>")
    // ([&bookings, &db_manager](int user_id) {
    //     return bookings.get_user_bookings(user_id, db_manager);
    // });

    // CROW_ROUTE(app_ref, "/bookings/<int>")
    //     .methods("DELETE"_method)([&bookings, &db_manager](int booking_id) {
    //         return bookings.cancel_booking(booking_id, db_manager);
    //     });
    // CROW_ROUTE(app_ref, "/rooms/<int>/availability")
    //     .methods("POST"_method
    //     )([&bookings, &db_manager](const crow::request &req, int room_id) {
    //         return bookings.create_availability(req, db_manager, room_id);
    //     });

    /* Buildings module - for future */

    // CROW_ROUTE(app, "/buildings")
    // ([&buildings]() { return buildings.get_all_buildings(); });
    // CROW_ROUTE(app, "/buildings/<int>")
    // ([&buildings](int id) { return buildings.get_building(id); });
    // CROW_ROUTE(app, "/buildings/<int>/rooms")
    // ([&buildings](int id) { return buildings.get_building_rooms(id); });
}
