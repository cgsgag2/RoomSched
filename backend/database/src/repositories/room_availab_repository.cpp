/***
 * Database room availability repository module (source).
 ***/

#include "repositories/room_availab_repository.hpp"
#include <exception>
#include <iostream>

roomsched::db::room_availab_repository::room_availab_repository(database &db_)
    : db(db_) {
}

bool roomsched::db::room_availab_repository::create_availab(
    const room_availability &room_av
) {
    try {
        db.execute(
            "INSERT INTO room_availability(room_id, date, is_available, "
            "available_from, available_to, booking_reference) "
            "VALUES($1, $2, $3, $4, $5, $6)",
            room_av.room_id, room_av.date, room_av.is_available,
            room_av.available_from, room_av.available_to, room_av.booking_ref
        );
        std::cout << "Room availability with index '" << room_av.room_id
                  << "' was created!" << std::endl;
        return true;
    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in create_availab]: " << e.what() << std::endl
                  << "Query: " << e.query() << std::endl;
        return false;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in create_availab]: " << e.what()
                  << std::endl;
        return false;
    }
}

void roomsched::db::room_availab_repository::update_availab(
    const room_availability &room_av
) {
    // TODO: later!
}

void roomsched::db::room_availab_repository::remove_availab(int room_id) {
    // TODO: later!
}

// Note: date must be in format 'YYYY-MM-DD', otherwise UB
std::optional<roomsched::db::room_availability>
roomsched::db::room_availab_repository::find_availab_by_room_and_date(
    int room_id,
    const std::string &date
) {
    room_availability avail;

    try {
        const auto result = db.query(
            "SELECT * FROM room_availability WHERE room_id = $1 AND date = $2",
            room_id, date
        );
        if (result.empty()) {
            std::cout << "Can't find availability for room with id: " << room_id
                      << " and date: " << date << std::endl;

            return avail;
        }
        const auto row = result[0];

        avail.room_id = row["room_id"].as<int>();
        avail.date = row["date"].as<std::string>();
        avail.is_available = row["is_available"].as<bool>();
        avail.available_from = row["available_from"].as<std::string>();
        avail.available_to = row["available_to"].as<std::string>();
        avail.booking_ref = row["booking_reference"].as<std::string>();

    } catch (const pqxx::unique_violation &) {
        std::cerr << "[NOTE]: room '" << room_id
                  << "' already has availability!" << std::endl;
    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in find_availab_by_room_and_date]: "
                  << e.what() << std::endl
                  << "Query: " << e.query() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in find_availab_by_room_and_date]: "
                  << e.what() << std::endl;
    }

    return avail;
}

std::vector<roomsched::db::room_availability>
roomsched::db::room_availab_repository::get_all_availabs_by_roomid(int room_id
) {
    std::vector<room_availability> all_avails;

    // TODO: when will think about different dates for one room, so
    // (room_id) is not unique, but (room_id, date, ?? from-to??) is unique.
    // First task - "find_availab_by_room_and_date" function.
    // Second task - this function.
    return all_avails;
}
