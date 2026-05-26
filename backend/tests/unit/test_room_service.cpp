/***
 * Test room service module (unit test).
 ***/

#include <gtest/gtest.h>
#include "fixtures/test_db_fixture.hpp"
#include "services/room_service.hpp"

namespace roomsched::db {

TEST_F(backend_db_test, RoomService_CreateRoom_Success) {
    room_service service(*global_db);

    room r;
    r.room_number = "254C";
    r.building = "New building";
    r.floor = 4;
    r.total_area = 30;
    r.description = "New testing building";
    r.type = room_type::COWORKING;
    r.total_capacity = 25;
    r.has_wifi = false;

    EXPECT_NO_THROW(service.create_room(r));
}

TEST_F(backend_db_test, RoomService_GetAllRooms) {
    room_service service(*global_db);

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
    service.create_room(lecture);

    room cowork;
    cowork.room_number = "212";
    cowork.building = "Канатный цех";
    cowork.floor = 2;
    cowork.total_area = 40;
    cowork.description = "Coworking zone";
    cowork.type = room_type::COWORKING;
    cowork.total_capacity = 25;
    cowork.has_wifi = true;
    service.create_room(cowork);

    auto rooms = service.get_all_rooms();

    ASSERT_EQ(rooms.size(), 2);
}

}  // namespace roomsched::db
