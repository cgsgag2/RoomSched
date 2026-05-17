/***
 * Test room repository module (unit test).
 ***/

#include <gtest/gtest.h>
#include "fixtures/test_db_fixture.hpp"
#include "repositories/room_repository.hpp"

namespace roomsched::db {

TEST_F(backend_db_test, RoomRepo_CreateRoom_Success) {
    room_repository repo(*global_db);

    room r;
    r.room_number = "254C";
    r.building = "New building";
    r.floor = 4;
    r.total_area = 30;
    r.description = "New testing building";
    r.type = room_type::COWORKING;
    r.total_capacity = 25;
    r.has_wifi = false;

    EXPECT_TRUE(repo.create_room(r));
}

TEST_F(backend_db_test, RoomRepo_GetRoomById_Found) {
    room_repository repo(*global_db);

    room r;
    r.room_number = "254C";
    r.building = "New building";
    r.floor = 4;
    r.total_area = 30;
    r.description = "New testing building";
    r.type = room_type::COWORKING;
    r.total_capacity = 25;
    r.has_wifi = false;

    repo.create_room(r);
    auto result = repo.get_room_by_id(1);

    EXPECT_EQ(result.room_number, "254C");
}

TEST_F(backend_db_test, RoomRepo_GetAll_Success) {
    room_repository repo(*global_db);

    room r;
    r.room_number = "254C";
    r.building = "New building";
    r.floor = 4;
    r.total_area = 30;
    r.description = "New testing building";
    r.type = room_type::COWORKING;
    r.total_capacity = 25;
    r.has_wifi = false;

    repo.create_room(r);

    room r1;
    r1.room_number = "100A";
    r1.building = "New building number 2";
    r1.floor = 5;
    r1.total_area = 10;
    r1.description = "New testing building 2";
    r1.type = room_type::LECTURE;

    repo.create_room(r);

    auto all = repo.get_all_rooms();

    ASSERT_FALSE(all.empty());

    EXPECT_EQ(all.size(), 2);
}

}  // namespace roomsched::db
