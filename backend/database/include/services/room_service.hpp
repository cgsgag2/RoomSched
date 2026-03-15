/***
 * Database room service module (header).
 ***/

#ifndef ROOM_SERVICE_HPP_
#define ROOM_SERVICE_HPP_

#include "../repositories/room_availab_repository.hpp"
#include "../repositories/room_repository.hpp"

// project namespace, database module namespace
namespace roomsched::db {
class room_service {
private:
    room_repository room_repo;
    room_availab_repository availability_repo;

public:
    room_service(database &db_);

    // Room service
    void create_room(const room &new_room);
    void create_default_rooms();

    room get_room_by_id(int id);
    std::vector<room> get_all_rooms();

    // Availability room service
    void set_availability(const room_availability &avail);

    std::optional<room_availability> get_availability_by_id_and_date(
        int room_id,
        const std::string &date = "2026-03-15"
    );  // Note: date must be in format 'YYYY-MM-DD', otherwise UB

    bool is_room_available(
        int room_id,
        const std::string &date,
        const std::string &start,
        const std::string &end
    );

    std::vector<room> find_available_rooms(
        const std::string &date,
        const std::string &start,
        const std::string &end
    );
};
}  // namespace roomsched::db

#endif  // ROOM_SERVICE_HPP_
