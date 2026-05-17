/***
 * Database building repository module (source).
 ***/

#include "repositories/building_repository.hpp"
#include <exception>
#include <iostream>

roomsched::db::building_repository::building_repository(database &db_) : db(db_) {
}

int roomsched::db::building_repository::create_building(
    const std::string &name,
    const std::string &address
) {
    try {
        const auto result = db.query(
            "INSERT INTO buildings(name, address) VALUES($1, $2) RETURNING id",
            name,
            address
        );
        return result[0]["id"].as<int>();
    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in create_building]: " << e.what() << std::endl
                  << "Query: " << e.query() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in create_building]: " << e.what()
                  << std::endl;
    }
    return -1;
}

std::optional<roomsched::db::building>
roomsched::db::building_repository::get_building_by_name(
    const std::string &name
) {
    try {
        const auto result =
            db.query("SELECT id, name, address FROM buildings WHERE name = $1", name);
        if (result.empty()) {
            return std::nullopt;
        }
        building current;
        current.id = result[0]["id"].as<int>();
        current.name = result[0]["name"].as<std::string>();
        current.address = result[0]["address"].as<std::string>("");
        return current;
    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in get_building_by_name]: " << e.what()
                  << std::endl
                  << "Query: " << e.query() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in get_building_by_name]: " << e.what()
                  << std::endl;
    }
    return std::nullopt;
}

std::vector<roomsched::db::building>
roomsched::db::building_repository::get_all_buildings() {
    std::vector<building> buildings;

    try {
        const auto result =
            db.query("SELECT id, name, address FROM buildings ORDER BY id");
        for (const auto &row : result) {
            building current;
            current.id = row["id"].as<int>();
            current.name = row["name"].as<std::string>();
            current.address = row["address"].as<std::string>("");
            buildings.push_back(current);
        }
    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in get_all_buildings]: " << e.what() << std::endl
                  << "Query: " << e.query() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in get_all_buildings]: " << e.what()
                  << std::endl;
    }
    return buildings;
}

