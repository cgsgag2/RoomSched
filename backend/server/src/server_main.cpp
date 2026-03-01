/***
 * Server main module (source).
 ***/

#include <crow/app.h>
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <crow/routing.h>
#include <iostream>
// Local libs
#include "db_manager.hpp"
#include "server_auth.hpp"
#include "server_bookings.hpp"
#include "server_buildings.hpp"

int main() {
    // DB init module
    roomsched::db::database_manager app_db;
    if (!app_db.init()) {
        std::cerr << "STOP PROGRAM." << std::endl;
        return 0;
    }

    // Main (server) module
    crow::SimpleApp app;
    app.loglevel(crow::LogLevel::Debug);

    roomsched::server::auth_handler auth;
    roomsched::server::buildings_handler buildings;
    roomsched::server::bookings_handler bookings;

    // Test module
    CROW_ROUTE(app, "/")([]() { return "Hello Diana and Alex :))"; });

    CROW_ROUTE(app, "/health")
    ([]() {
        crow::json::wvalue result;
        result["status"] = "OK";
        result["message"] = "Server is running";
        return result;
    });

    CROW_ROUTE(app, "/test")
    ([]() {
        crow::json::wvalue response;
        response["name"] = "Varvara";
        response["project"] = "RoomSched";
        response["version"] = 0.1;
        return response;
    });

    // Module that works with DB
    CROW_ROUTE(app, "/register")
        .methods("POST"_method)([&auth, &app_db](const crow::request &req) {
            return auth.handle_register(req, app_db);
        });

    CROW_ROUTE(app, "/login")
        .methods("POST"_method)([&auth, &app_db](const crow::request &req) {
            return auth.handle_login(req, app_db);
        });

    CROW_ROUTE(app, "/get_users")
    ([&auth, &app_db]() { return auth.get_all_users(app_db); });

    // Module that will not be used yet
    CROW_ROUTE(app, "/buildings")
    ([&buildings]() { return buildings.get_all_buildings(); });
    CROW_ROUTE(app, "/buildings/<int>")
    ([&buildings](int id) { return buildings.get_building(id); });
    CROW_ROUTE(app, "/buildings/<int>/rooms")
    ([&buildings](int id) { return buildings.get_building_rooms(id); });

    CROW_ROUTE(app, "/bookings")
        .methods("POST"_method)([&bookings, &app_db](const crow::request &req) {
            return bookings.create_booking(req, app_db);
        });

    CROW_ROUTE(app, "/bookings/user/<int>")
    ([&bookings, &app_db](int user_id) {
        return bookings.get_user_bookings(user_id, app_db);
    });

    CROW_ROUTE(app, "/bookings/<int>")
        .methods("DELETE"_method)([&bookings, &app_db](int booking_id) {
            return bookings.cancel_booking(booking_id, app_db);
        });
    CROW_ROUTE(app, "/rooms/<int>/availability")
        .methods("POST"_method
        )([&bookings, &app_db](const crow::request &req, int room_id) {
            return bookings.create_availability(req, app_db, room_id);
        });

    std::cout << "=================================" << std::endl;
    std::cout << "SERVER STARTING :)" << std::endl;
    std::cout << "=================================" << std::endl;

    app.port(8080).multithreaded().run();
}
