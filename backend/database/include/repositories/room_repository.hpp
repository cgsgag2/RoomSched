/***
 * Database room repository module (header).
 ***/

#ifndef ROOM_REPOSITORY_HPP_
#define ROOM_REPOSITORY_HPP_

#include <optional>
#include "../core/db_core.hpp"
#include "../models/room_model.hpp"

// project namespace, database module namespace
namespace roomsched::db {
class room_repository {
private:
    database &db;

    void create_lecture_room(int room_id, const room &r);
    void create_coworking_space(int room_id, const room &r);
    void create_private_office(int room_id, const room &r);

public:
    room_repository(database &db_);

    int create_room(const room &new_room);
    std::optional<room> get_room_by_id(int id);
    std::vector<room> get_all_rooms();
    std::vector<room> get_all_rooms_detailed();
};
}  // namespace roomsched::db

#endif  // ROOM_REPOSITORY_HPP_
