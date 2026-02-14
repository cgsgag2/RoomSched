#include <crow/app.h>
#include <crow/json.h>
#include "buildings.hpp" 
#include <crow/routing.h>
#include <crow/http_request.h>
#include <crow/http_response.h>
#include "auth.hpp"
#include <iostream>

int main() {
    crow::SimpleApp app;

    roomsched::AuthHandler auth;
    roomsched::BuildingsHandler buildings;

    CROW_ROUTE(app, "/")([](){
        return "Hello Diana and Alex :))";
    });

    CROW_ROUTE(app, "/health")([](){
        crow::json::wvalue result;
        result["status"] = "OK";
        result["message"] = "Server is running";
        return result;
    });

    CROW_ROUTE(app, "/test")([](){
        crow::json::wvalue response;
        response["name"] = "Varvara";
        response["project"] = "RoomSched";
        response["version"] = 0.1;
        return response;
    });
    CROW_ROUTE(app, "/register").methods("POST"_method)([&auth](const crow::request& req){
        return auth.handle_register(req);
    });
    CROW_ROUTE(app, "/login").methods("POST"_method)([&auth](const crow::request& req){
        return auth.handle_login(req);
    });
    CROW_ROUTE(app, "/buildings")([&buildings](){
        return buildings.get_all_buildings();
    });
    CROW_ROUTE(app, "/buildings/<int>")([&buildings](int id){
        return buildings.get_building(id);
    });
    CROW_ROUTE(app, "/buildings/<int>/rooms")([&buildings](int id){
        return buildings.get_building_rooms(id);
    });


    std::cout << "=================================" << std::endl;
    std::cout << "SERVER STARTING :)" << std::endl;
    std::cout << "=================================" << std::endl;

    app.port(8080).multithreaded().run();
}