/***
 * Database building service module (header).
 ***/

#ifndef BUILDING_SERVICE_HPP_
#define BUILDING_SERVICE_HPP_

#include "../repositories/building_repository.hpp"

// project namespace, database module namespace
namespace roomsched::db {
class building_service {
private:
    building_repository repo;

public:
    building_service(database &db_);

    int ensure_building(const std::string &name, const std::string &address);
    void create_default_buildings();
    std::vector<building> get_all_buildings();
};
}  // namespace roomsched::db

#endif  // BUILDING_SERVICE_HPP_
