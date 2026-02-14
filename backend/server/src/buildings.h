#pragma once
#include "crow/http_response.h"
#include "crow/json.h"

crow::response get_all_buildings();
crow::response get_building(int id);
crow::response get_building_rooms(int building_id);