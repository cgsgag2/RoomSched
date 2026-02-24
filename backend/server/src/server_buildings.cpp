/***
 * Server module for buildings (source).
 ***/

#include "server_buildings.hpp"
#include "server_json_utils.hpp"

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
crow::response roomsched::server::buildings_handler::get_all_buildings() {
    crow::json::wvalue data;
    data["buildings"] = crow::json::wvalue::list(
        {crow::json::wvalue{
             {"id", 1}, {"name", "Коворкинг"}, {"type", "public"}},
         crow::json::wvalue{
             {"id", 2}, {"name", "Корпус НИУ ВШЭ"}, {"type", "public"}}}
    );

    return json_utils::success_response("Buildings revealed", data);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
crow::response roomsched::server::buildings_handler::get_building(int id) {
    crow::json::wvalue data;
    if (id == 1) {
        data["id"] = id;
        data["name"] = "Коворкинг";
        data["type"] = "public";
    } else if (id == 2) {
        data["id"] = id;
        data["name"] = "Корпус НИУ ВШЭ";
        data["type"] = "public";
    } else {
        return json_utils::error_response("Building not found", 404);
    }

    return json_utils::success_response("Building revealed", data);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
crow::response roomsched::server::buildings_handler::get_building_rooms(
    int building_id
) {
    crow::json::wvalue data;
    data["building_id"] = building_id;
    if (building_id == 1) {
        data["rooms"] = crow::json::wvalue::list(
            {crow::json::wvalue{
                 {"id", 101}, {"name", "Переговорная 1-2"}, {"capacity", 2}},
             crow::json::wvalue{
                 {"id", 102}, {"name", "Переговорная 3-4"}, {"capacity", 4}},
             crow::json::wvalue{
                 {"id", 103}, {"name", "Лекторий"}, {"capacity", 65}}}
        );
    } else if (building_id == 2) {
        data["rooms"] = crow::json::wvalue::list(
            {crow::json::wvalue{
                 {"id", 213}, {"name", "Лекционная"}, {"capacity", 150}},
             crow::json::wvalue{
                 {"id", 224}, {"name", "Аудитория 224"}, {"capacity", 20}},
             crow::json::wvalue{
                 {"id", 239}, {"name", "Аудитория 239"}, {"capacity", 2}},
             crow::json::wvalue{
                 {"id", 207}, {"name", "Компьютерный класс"}, {"capacity", 15}}}
        );
    } else {
        return json_utils::error_response("Building not found", 404);
    }

    return json_utils::success_response("Rooms revealed", data);
}
