/***
 * Server main module (source).
 ***/

#include "server/app.hpp"

int main() {
    roomsched::server::app main_app;

    // Small test for users
    // roomsched::db::user user1;
    // user1.email = "test@example.com";
    // user1.phone = "1234567890";
    // user1.password_hash = "password123";
    // user1.full_name = "testuser";

    // main_app.get_db().users().register_user(
    //     user1.email, user1.password_hash, user1.full_name, user1.phone
    // );

    main_app.run(8080);

#if 0
    // This must be TEST MODULE
    // Small test for rooms
    db_manager.rooms().create_default_rooms();
    roomsched::db::room new_lecture;
    new_lecture.room_number = "1000";
    new_lecture.building = "Kanatny";
    new_lecture.floor = 10;
    new_lecture.total_area = 100;
    new_lecture.description = "new lecture";
    new_lecture.type = roomsched::db::room_type::LECTURE;
    new_lecture.capacity = 10;
    new_lecture.has_projector = false;
    new_lecture.has_whiteboard = false;
    db_manager.rooms().create_room(new_lecture);
    std::cout << "-----------Test: get room by ID----------------" << std::endl;
    roomsched::db::room test_get_room = db_manager.rooms().get_room_by_id(1);
    std::cout << "id: " << test_get_room.id
              << ", room_number: " << test_get_room.room_number
              << ", building: " << test_get_room.building
              << "\ntotal area: " << test_get_room.total_area
              << ", description: " << test_get_room.description << std::endl;
    std::cout << "has projector?: ";
    if (test_get_room.has_projector) {
        std::cout << *test_get_room.has_projector;
    } else {
        std::cout << "false";
    }
    std::cout << ", capacity: ";
    if (test_get_room.capacity) {
        std::cout << *test_get_room.capacity;
    } else {
        std::cout << "None (not this type)";
    }
    std::cout << ", number of chairs: ";
    if (test_get_room.number_of_chairs) {
        std::cout << *test_get_room.number_of_chairs;
    } else {
        std::cout << "None (not this type)";
    }
    std::cout << std::endl;

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
