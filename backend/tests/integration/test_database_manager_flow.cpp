/***
 * Test database manager flow (integration test).
 ***/

#if 0

#include <gtest/gtest.h>
#include "db_manager.hpp"
#include "fixtures/test_db_fixture.hpp"

namespace roomsched::db {

TEST_F(backend_db_test, DatabaseManager_FullFlow) {
    db_config config(
        "roomsched_test", "rsched_user_test", "password_tests_roomsched",
        "localhost"
    );

    database_manager manager(config);

    auto u = manager.users().register_user(
        "manager@gmail.com", "pwd1233434", "Manager User", "123"
    );
    EXPECT_TRUE(u.success);

    room r;
    r.room_number = "254C";
    r.building = "New building";
    r.floor = 4;
    r.total_area = 30;
    r.description = "New testing building";
    r.type = room_type::COWORKING;
    r.total_capacity = 25;
    r.has_wifi = false;
    EXPECT_NO_THROW(manager.rooms().create_room(r));

    auto user = manager.users().login("manager@gmail.com", "pwd1233434");
    auto room = manager.rooms().get_room_by_id(1);

    ASSERT_TRUE(user.has_value());
    ASSERT_TRUE(room.has_value());

    auto res = manager.bookings().create_booking(
        user->id, room->id, "2025-01-01", "10:00:00", "11:00:00"
    );
    EXPECT_TRUE(res.has_value());
}

}  // namespace roomsched::db

#endif  // 0
