#include "buildings.hpp"
#include "json_utils.hpp"

crow::response roomsched::BuildingsHandler::get_all_buildings() {
    crow::json::wvalue data;
    data["buildings"] = crow::json::wvalue::list({
        crow::json::wvalue{
            {"id", 1},
            {"name", "Коворкинг"},
            {"type", "public"}
        },
        crow::json::wvalue{
            {"id", 2},
            {"name", "Корпус НИУ ВШЭ"},
            {"type", "public"}
        }
    });
    
    return roomsched::json_utils::success_response("Buildings revealed", data);
}

crow::response roomsched::BuildingsHandler::get_building(int id) {
    crow::json::wvalue data;
    if (id == 1){
        data["id"] = id;
        data["name"] = "Коворкинг";
        data["type"] = "public";
    }
    else if (id==2){
        data["id"] = id;
        data["name"] = "Корпус НИУ ВШЭ";
        data["type"] = "public";
    }
    else {
        return roomsched::json_utils::error_response("Building not found", 404);
    }
    
    
    return roomsched::json_utils::success_response("Building revealed", data);
}

crow::response roomsched::BuildingsHandler::get_building_rooms(int building_id) {
    crow::json::wvalue data;
    data["building_id"] = building_id;
    if (building_id == 1){
        data["rooms"] = crow::json::wvalue::list({
        crow::json::wvalue{
            {"id", 101},
            {"name", "Переговорная 1-2"},
            {"capacity", 2}
        },
        crow::json::wvalue{
            {"id", 102},
            {"name", "Переговорная 3-4"},
            {"capacity", 4}
        },
        crow::json::wvalue{
            {"id", 103},
            {"name", "Лекторий"},
            {"capacity", 65}
        }
    });
    }
    else if (building_id == 2){
        data["rooms"] = crow::json::wvalue::list({
        crow::json::wvalue{
            {"id", 213},
            {"name", "Лекционная"},
            {"capacity", 150}
        },
        crow::json::wvalue{
            {"id", 224},
            {"name", "Аудитория 224"},
            {"capacity", 20}
        },
        crow::json::wvalue{
            {"id", 239},
            {"name", "Аудитория 239"},
            {"capacity", 2}
        },
        crow::json::wvalue{
            {"id", 207},
            {"name", "Компьютерный класс"},
            {"capacity", 15}
        }
    });
    }
    else {
        return roomsched::json_utils::error_response("Building not found", 404);
    }
    
    
    return roomsched::json_utils::success_response("Rooms revealed", data);
}

