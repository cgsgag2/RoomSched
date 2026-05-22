/***
 * Database building service module (source).
 ***/

#include "services/building_service.hpp"

roomsched::db::building_service::building_service(database &db_) : repo(db_) {
}

int roomsched::db::building_service::ensure_building(
    const std::string &name,
    const std::string &address
) {
    auto existing = repo.get_building_by_name(name);
    if (existing.has_value()) {
        return existing->id;
    }
    return repo.create_building(name, address);
}

void roomsched::db::building_service::create_default_buildings() {
    ensure_building("Канатный цех", "");
    ensure_building("Кантемировская", "");
    ensure_building("Седова", "");
    ensure_building("Грибоедова", "");
}

std::vector<roomsched::db::building>
roomsched::db::building_service::get_all_buildings() {
    return repo.get_all_buildings();
}
