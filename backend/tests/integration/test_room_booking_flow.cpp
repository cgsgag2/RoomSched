/***
 * Test room booking flow (integration test).
 ***/

#include <gtest/gtest.h>
#include "fixtures/test_db_fixture.hpp"
#include "services/booking_service.hpp"
#include "services/room_service.hpp"
#include "services/user_service.hpp"

namespace roomsched::db {

TEST_F(backend_db_test, RoomBookingFlow_CreateConflictCancel) {
    user_service users(*global_db);
    room_service rooms(*global_db);
    booking_service bookings(*global_db, rooms);

    users.register_user("flow@gmail.com", "pwd123shgdf!", "Flow User", "123");

    room r;
    r.room_number = "254C";
    r.building = "New building";
    r.floor = 4;
    r.total_area = 30;
    r.description = "New testing building";
    r.type = room_type::COWORKING;
    r.total_capacity = 25;
    r.has_wifi = false;
    rooms.create_room(r);

    auto user = users.login("flow@gmail.com", "pwd123shgdf!");
    auto room = rooms.get_room_by_id(1);

    ASSERT_TRUE(user.has_value());
    ASSERT_TRUE(room.has_value());

    auto res = bookings.create_booking(
        user->id, room->id, "2025-01-01", "10:00:00", "11:00:00"
    );
    EXPECT_TRUE(res.has_value());

    auto not_correct = bookings.create_booking(
        user->id, room->id, "2025-01-01", "10:00:00", "11:00:00"
    );
    EXPECT_FALSE(not_correct.has_value());
}

}  // namespace roomsched::db
