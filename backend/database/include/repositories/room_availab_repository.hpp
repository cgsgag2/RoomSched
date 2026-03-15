/***
 * Database room availability repository module (header).
 ***/

#ifndef ROOM_AVAILAB_REPOSITORY_HPP_
#define ROOM_AVAILAB_REPOSITORY_HPP_

#include "../core/db_core.hpp"
#include "../models/room_model.hpp"

// project namespace, database module namespace
namespace roomsched::db {
class room_availab_repository {
private:
    database &db;

public:
    room_availab_repository(database &db_);

    bool create_availab(const room_availability &room_av);
    // TODO: later (maybe not in MVP)
    void update_availab(const room_availability &room_av);
    void remove_availab(int room_id);

    // Note: date must be in format 'YYYY-MM-DD', otherwise UB
    std::optional<room_availability>
    find_availab_by_room_and_date(int room_id, const std::string &date);

    // TODO: later (maybe not in MVP)
    std::vector<room_availability> get_all_availabs_by_roomid(int room_id);
};
}  // namespace roomsched::db

#endif  // ROOM_AVAILAB_REPOSITORY_HPP_
