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
    roomsched::db::db_config default_config;
    roomsched::db::database_manager db_manager(default_config);

    // This is part that CAN BE COMMENTED!!

    // Small test for users
    roomsched::db::user user1;
    user1.username = "Sanya";
    user1.password = "password123";
    db_manager.users().register_user(user1);

    // Small test for rooms
    db_manager.rooms().create_default_rooms();
    roomsched::db::room new_lecture;
    new_lecture.room_number = "1000";
    new_lecture.building = "Kanatny";
    new_lecture.floor = 10;
    new_lecture.total_area = 100;
    new_lecture.description = "new lecture";
    new_lecture.type = roomsched::db::room_type::LECTURE;
    new_lecture.capacity = 10;
    new_lecture.has_projector = false;
    new_lecture.has_whiteboard = false;
    db_manager.rooms().create_room(new_lecture);
    std::cout << "-----------Test: get room by ID----------------" << std::endl;
    roomsched::db::room test_get_room = db_manager.rooms().get_room_by_id(1);
    std::cout << "id: " << test_get_room.id
              << ", room_number: " << test_get_room.room_number
              << ", building: " << test_get_room.building
              << "\ntotal area: " << test_get_room.total_area
              << ", description: " << test_get_room.description << std::endl;
    std::cout << "has projector?: ";
    if (test_get_room.has_projector) {
        std::cout << *test_get_room.has_projector;
    } else {
        std::cout << "false";
    }
    std::cout << ", capacity: ";
    if (test_get_room.capacity) {
        std::cout << *test_get_room.capacity;
    } else {
        std::cout << "None (not this type)";
    }
    std::cout << ", number of chairs: ";
    if (test_get_room.number_of_chairs) {
        std::cout << *test_get_room.number_of_chairs;
    } else {
        std::cout << "None (not this type)";
    }
    std::cout << std::endl;

    // Small test for room availability
    roomsched::db::room_availability roomav1;
    roomav1.room_id = 1;
    db_manager.rooms().set_availability(roomav1);

    roomsched::db::room_availability roomav2;
    roomav2.room_id = 2;
    roomav2.available_from = "08:00";
    roomav2.available_to = "15:00";
    db_manager.rooms().set_availability(roomav2);

    roomsched::db::room_availability roomav3;
    roomav3.room_id = 3;
    roomav3.is_available = false;
    db_manager.rooms().set_availability(roomav3);

    std::cout << "-----------Test: get room availability----------------"
              << std::endl;
    roomsched::db::room_availability info_av;
    info_av = *db_manager.rooms().get_availability_by_id_and_date(1);
    std::cout << "room_id: " << info_av.room_id << ", date: " << info_av.date
              << ", is_availab: " << info_av.is_available
              << "\nabailable from: " << info_av.available_from
              << ", to: " << info_av.available_to
              << ", booking ref-info: " << info_av.booking_ref << std::endl;

    std::cout << "-----------Test: check room availability----------------"
              << std::endl;
    std::cout << "Room 3: is available? "
              << db_manager.rooms().is_room_available(
                     3, "2026-03-15", "10:00", "15:00"
                 )
              << std::endl;
    std::vector<roomsched::db::room> avail_rooms;
    avail_rooms =
        db_manager.rooms().find_available_rooms("2026-03-15", "14:30", "17:30");
    for (const auto r : avail_rooms) {
        std::cout << "id: " << r.id << ", room_number: " << r.room_number
                  << ", building: " << r.building
                  << "\ntotal area: " << r.total_area
                  << ", description: " << r.description << std::endl;
    }
    // End of part that CAN BE COMMENTED!!

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
        .methods("POST"_method)([&auth, &db_manager](const crow::request &req) {
            return auth.handle_register(req, db_manager);
        });

    CROW_ROUTE(app, "/login")
        .methods("POST"_method)([&auth, &db_manager](const crow::request &req) {
            return auth.handle_login(req, db_manager);
        });

    CROW_ROUTE(app, "/get_users")
    ([&auth, &db_manager]() { return auth.get_all_users(db_manager); });

    // Module that will not be used yet
    CROW_ROUTE(app, "/buildings")
    ([&buildings]() { return buildings.get_all_buildings(); });
    CROW_ROUTE(app, "/buildings/<int>")
    ([&buildings](int id) { return buildings.get_building(id); });
    CROW_ROUTE(app, "/buildings/<int>/rooms")
    ([&buildings](int id) { return buildings.get_building_rooms(id); });

    CROW_ROUTE(app, "/bookings")
        .methods("POST"_method
        )([&bookings, &db_manager](const crow::request &req) {
            return bookings.create_booking(req, db_manager);
        });

    CROW_ROUTE(app, "/bookings/user/<int>")
    ([&bookings, &db_manager](int user_id) {
        return bookings.get_user_bookings(user_id, db_manager);
    });

    CROW_ROUTE(app, "/bookings/<int>")
        .methods("DELETE"_method)([&bookings, &db_manager](int booking_id) {
            return bookings.cancel_booking(booking_id, db_manager);
        });
    CROW_ROUTE(app, "/rooms/<int>/availability")
        .methods("POST"_method
        )([&bookings, &db_manager](const crow::request &req, int room_id) {
            return bookings.create_availability(req, db_manager, room_id);
        });

    std::cout << "=================================" << std::endl;
    std::cout << "SERVER STARTING :)" << std::endl;
    std::cout << "=================================" << std::endl;

    app.port(8080).multithreaded().run();
}
