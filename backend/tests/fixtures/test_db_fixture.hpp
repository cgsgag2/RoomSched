/***
 * Test database fixture module (header).
 ***/

#ifndef TEST_DB_FIXTURE_HPP_
#define TEST_DB_FIXTURE_HPP_

#include <gtest/gtest.h>
#include "test_global_db.hpp"

class backend_db_test : public ::testing::Test {
protected:
    void SetUp() override {
        global_db->execute(
            "TRUNCATE users, rooms_all, lecture_room, coworking_space, "
            "private_office, room_availability, room_booking RESTART IDENTITY "
            "CASCADE"
        );
    }
};

#endif  // TEST_DB_FIXTURE_HPP_
