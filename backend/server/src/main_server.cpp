#include "crow/app.h"
#include "crow/json.h"
#include "buildings.h" 
#include "crow/routing.h"
#include "crow/http_request.h"
#include "crow/http_response.h"
#include "auth.h"
#include <iostream>

int main() {
    crow::SimpleApp app;

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
    CROW_ROUTE(app, "/register").methods("POST"_method)(handle_register);
    CROW_ROUTE(app, "/login").methods("POST"_method)(handle_login);

    CROW_ROUTE(app, "/buildings")(get_all_buildings);
    CROW_ROUTE(app, "/buildings/<int>")(get_building);
    CROW_ROUTE(app, "/buildings/<int>/rooms")(get_building_rooms);


    std::cout << "=================================" << std::endl;
    std::cout << "SERVER STARTING :)" << std::endl;
    std::cout << "=================================" << std::endl;

    app.port(8080).multithreaded().run();
}