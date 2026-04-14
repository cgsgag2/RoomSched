/***
 * Server main module (source).
 ***/

#include "server/app.hpp"

int main() {
    roomsched::server::app main_app;

    main_app.run(8080);

#if 0
    // This must be TEST MODULE
    // Small test for room availability
    roomsched::db::room_availability roomav1;
    roomav1.room_id = 1;
    db_manager.rooms().set_availability(roomav1);

    roomsched::db::room_availability roomav2;
    roomav2.room_id = 2;
    roomav2.available_from = "08:00";
    roomav2.available_to = "15:00";
    db_manager.rooms().set_availability(roomav2);

    roomsched::db::room_availability roomav3;
    roomav3.room_id = 3;
    roomav3.is_available = false;
    db_manager.rooms().set_availability(roomav3);

    std::cout << "-----------Test: get room availability----------------"
              << std::endl;
    roomsched::db::room_availability info_av;
    info_av = *db_manager.rooms().get_availability_by_id_and_date(1);
    std::cout << "room_id: " << info_av.room_id << ", date: " << info_av.date
              << ", is_availab: " << info_av.is_available
              << "\nabailable from: " << info_av.available_from
              << ", to: " << info_av.available_to
              << ", booking ref-info: " << info_av.booking_ref << std::endl;

    std::cout << "-----------Test: check room availability----------------"
              << std::endl;
    std::cout << "Room 3: is available? "
              << db_manager.rooms().is_room_available(
                     3, "2026-03-15", "10:00", "15:00"
                 )
              << std::endl;
    std::vector<roomsched::db::room> avail_rooms;
    avail_rooms =
        db_manager.rooms().find_available_rooms("2026-03-15", "14:30", "17:30");
    for (const auto r : avail_rooms) {
        std::cout << "id: " << r.id << ", room_number: " << r.room_number
                  << ", building: " << r.building
                  << "\ntotal area: " << r.total_area
                  << ", description: " << r.description << std::endl;
    }
    // End of part that CAN BE COMMENTED!!
#endif  // 0
}
