/***
 * Test database manager module (unit test).
 ***/

#include <gtest/gtest.h>
#include "db_manager.hpp"

namespace roomsched::db {

TEST(DatabaseManager, ServicesAccessible) {
    db_config config;

    database_manager manager(config);

    EXPECT_NO_THROW(manager.users());
    EXPECT_NO_THROW(manager.rooms());
    EXPECT_NO_THROW(manager.bookings());
}

}  // namespace roomsched::db
