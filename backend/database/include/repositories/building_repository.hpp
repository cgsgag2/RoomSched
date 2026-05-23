/***
 * Database building repository module (header).
 ***/

#ifndef BUILDING_REPOSITORY_HPP_
#define BUILDING_REPOSITORY_HPP_

#include "../core/db_core.hpp"
#include "../models/building_model.hpp"
#include <optional>
#include <vector>

// project namespace, database module namespace
namespace roomsched::db {
class building_repository {
private:
    database &db;

public:
    building_repository(database &db_);

    int create_building(const std::string &name, const std::string &address);
    std::optional<building> get_building_by_name(const std::string &name);
    std::vector<building> get_all_buildings();
};
}  // namespace roomsched::db

#endif  // BUILDING_REPOSITORY_HPP_

