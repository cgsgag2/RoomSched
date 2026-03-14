/***
 * Database room service module (source).
 ***/

#include "services/room_service.hpp"
#include <iostream>

roomsched::db::room_service::room_service(database &db_) : repo(db_) {
}

void roomsched::db::room_service::create_room(const room &new_room) {
    repo.create_room(new_room);
}

void roomsched::db::room_service::create_default_rooms() {
    room lecture;
    lecture.room_number = "213";
    lecture.building = "Kanatny tsekh";
    lecture.floor = 2;
    lecture.total_area = 50;
    lecture.description = "for AMI students";
    lecture.type = room_type::LECTURE;
    lecture.capacity = 80;
    lecture.has_projector = true;
    lecture.has_whiteboard = true;
    repo.create_room(lecture);

    room cowork;
    cowork.room_number = "212";
    cowork.building = "Kanta";
    cowork.floor = 3;
    cowork.total_area = 40;
    cowork.description = "Coworking zone";
    cowork.type = room_type::COWORKING;
    cowork.total_capacity = 25;
    cowork.has_wifi = true;
    repo.create_room(cowork);

    room office;
    office.room_number = "301";
    office.building = "Beringa street 20";
    office.floor = 10;
    office.total_area = 20;
    office.description = "Private office for Yandex";
    office.type = room_type::PRIVATE_OFFICE;
    office.number_of_chairs = 3;
    repo.create_room(office);
}

roomsched::db::room roomsched::db::room_service::get_room_by_id(int id) {
    return repo.get_room_by_id(id);
}

std::vector<roomsched::db::room> roomsched::db::room_service::get_all_rooms() {
    return repo.get_all_rooms();
}
