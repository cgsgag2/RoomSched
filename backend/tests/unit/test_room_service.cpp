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

    room r;
    r.room_number = "254C";
    r.building = "New building";
    r.floor = 4;
    r.total_area = 30;
    r.description = "New testing building";
    r.type = room_type::COWORKING;
    r.total_capacity = 25;
    r.has_wifi = false;
    service.create_room(r);

    room r1;
    r1.room_number = "B";
    r1.building = "New building 2";
    r1.floor = 3;
    r1.total_area = 14;
    r1.description = "New testing building";
    r1.type = room_type::PRIVATE_OFFICE;
    service.create_room(r1);

    auto rooms = service.get_all_rooms();

    ASSERT_EQ(rooms.size(), 2);
}

}  // namespace roomsched::db
