/***
 * Database room service module (header).
 ***/

#ifndef ROOM_SERVICE_HPP_
#define ROOM_SERVICE_HPP_

#include "../repositories/room_repository.hpp"

// project namespace, database module namespace
namespace roomsched::db {
class room_service {
private:
    room_repository repo;

public:
    room_service(database &db_);

    void create_room(const room &new_room);
    void create_default_rooms();

    room get_room_by_id(int id);
    std::vector<room> get_all_rooms();
};
}  // namespace roomsched::db

#endif  // ROOM_SERVICE_HPP_
