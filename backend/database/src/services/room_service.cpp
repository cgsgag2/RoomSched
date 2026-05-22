/***
 * Database room service module (source).
 ***/

#include "services/room_service.hpp"
#include <iostream>

roomsched::db::room_service::room_service(database &db_)
    : room_repo(db_), availability_repo(db_) {
}

// Room service

void roomsched::db::room_service::create_room(const room &new_room) {
    room_repo.create_room(new_room);
}

void roomsched::db::room_service::create_default_rooms() {
    room lecture;
    lecture.room_number = "213";
    lecture.building = "Канатный цех";
    lecture.floor = 2;
    lecture.total_area = 50;
    lecture.description = "for AMI students";
    lecture.type = room_type::LECTURE;
    lecture.capacity = 80;
    lecture.has_projector = true;
    lecture.has_whiteboard = true;
    room_repo.create_room(lecture);

    lecture.room_number = "109";
    lecture.building = "Канатный цех";
    lecture.floor = 1;
    lecture.total_area = 25;
    lecture.description = "for ADA_AI students";
    lecture.type = room_type::LECTURE;
    lecture.capacity = 100;
    lecture.has_projector = true;
    lecture.has_whiteboard = false;
    room_repo.create_room(lecture);

    lecture.room_number = "315";
    lecture.building = "Канатный цех";
    lecture.floor = 3;
    lecture.total_area = 100;
    lecture.description = "for all students";
    lecture.type = room_type::LECTURE;
    lecture.capacity = 120;
    lecture.has_projector = true;
    lecture.has_whiteboard = true;
    room_repo.create_room(lecture);

    room cowork;
    cowork.room_number = "212";
    cowork.building = "Канатный цех";
    cowork.floor = 2;
    cowork.total_area = 40;
    cowork.description = "Coworking zone";
    cowork.type = room_type::COWORKING;
    cowork.total_capacity = 25;
    cowork.has_wifi = true;
    room_repo.create_room(cowork);

    cowork.room_number = "125";
    cowork.building = "Канатный цех";
    cowork.floor = 1;
    cowork.total_area = 70;
    cowork.description = "Coworking zone";
    cowork.type = room_type::COWORKING;
    cowork.total_capacity = 25;
    cowork.has_wifi = false;
    room_repo.create_room(cowork);

    room office;
    office.room_number = "301";
    office.building = "Канатный цех";
    office.floor = 1;
    office.total_area = 20;
    office.description = "Private office for Yandex";
    office.type = room_type::PRIVATE_OFFICE;
    office.number_of_chairs = 3;
    room_repo.create_room(office);

    office.room_number = "11";
    office.building = "Канатный цех";
    office.floor = 1;
    office.total_area = 30;
    office.description = "Private office for T-Bank";
    office.type = room_type::PRIVATE_OFFICE;
    office.number_of_chairs = 10;
    room_repo.create_room(office);

    office.room_number = "324";
    office.building = "Канатный цех";
    office.floor = 3;
    office.total_area = 20;
    office.description = "Private office for Avito";
    office.type = room_type::PRIVATE_OFFICE;
    office.number_of_chairs = 5;
    room_repo.create_room(office);

    // Кантемировская
    lecture.room_number = "201";
    lecture.building = "Кантемировская";
    lecture.floor = 2;
    lecture.total_area = 45;
    lecture.description = "Lecture room in Кантемировская";
    lecture.type = room_type::LECTURE;
    lecture.capacity = 60;
    lecture.has_projector = true;
    lecture.has_whiteboard = true;
    room_repo.create_room(lecture);

    cowork.room_number = "210";
    cowork.building = "Кантемировская";
    cowork.floor = 2;
    cowork.total_area = 35;
    cowork.description = "Coworking zone in Кантемировская";
    cowork.type = room_type::COWORKING;
    cowork.total_capacity = 20;
    cowork.has_wifi = true;
    room_repo.create_room(cowork);

    // Седова
    lecture.room_number = "302";
    lecture.building = "Седова";
    lecture.floor = 3;
    lecture.total_area = 55;
    lecture.description = "Lecture room in Седова";
    lecture.type = room_type::LECTURE;
    lecture.capacity = 70;
    lecture.has_projector = true;
    lecture.has_whiteboard = false;
    room_repo.create_room(lecture);

    office.room_number = "310";
    office.building = "Седова";
    office.floor = 3;
    office.total_area = 25;
    office.description = "Private office in Седова";
    office.type = room_type::PRIVATE_OFFICE;
    office.number_of_chairs = 4;
    room_repo.create_room(office);

    // Грибоедова
    lecture.room_number = "401";
    lecture.building = "Грибоедова";
    lecture.floor = 4;
    lecture.total_area = 50;
    lecture.description = "Lecture room in Грибоедова";
    lecture.type = room_type::LECTURE;
    lecture.capacity = 65;
    lecture.has_projector = false;
    lecture.has_whiteboard = true;
    room_repo.create_room(lecture);

    cowork.room_number = "420";
    cowork.building = "Грибоедова";
    cowork.floor = 4;
    cowork.total_area = 40;
    cowork.description = "Coworking zone in Грибоедова";
    cowork.type = room_type::COWORKING;
    cowork.total_capacity = 30;
    cowork.has_wifi = true;
    room_repo.create_room(cowork);
}

std::optional<roomsched::db::room> roomsched::db::room_service::get_room_by_id(
    int id
) {
    return room_repo.get_room_by_id(id);
}

std::vector<roomsched::db::room> roomsched::db::room_service::get_all_rooms() {
    return room_repo.get_all_rooms();
}

// Availability room service

void roomsched::db::room_service::set_availability(
    const db::room_availability &avail
) {
    availability_repo.create_availab(avail);
}

std::optional<roomsched::db::room_availability>
roomsched::db::room_service::get_availability_by_id_and_date(
    int room_id,
    const std::string &date
) {
    return availability_repo.find_availab_by_room_and_date(room_id, date);
}

bool roomsched::db::room_service::is_room_available(
    int room_id,
    const std::string &date,
    const std::string &start,
    const std::string &end
) {
    auto avail = availability_repo.find_availab_by_room_and_date(room_id, date);
    if (!avail || !avail->is_available) {
        return false;
    }
    if (start < avail->available_from || end > avail->available_to) {
        return false;
    }
    return true;
}

std::vector<roomsched::db::room>
roomsched::db::room_service::find_available_rooms(
    const std::string &date,
    const std::string &start,
    const std::string &end
) {
    std::vector<room> all_avail_rooms;
    std::vector<room> rooms = room_repo.get_all_rooms();

    for (const auto &r : rooms) {
        if (is_room_available(r.id, date, start, end)) {
            all_avail_rooms.push_back(r);
        }
    }

    return all_avail_rooms;
}
