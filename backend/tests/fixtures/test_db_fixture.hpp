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
        global_db->execute("TRUNCATE users RESTART IDENTITY CASCADE");
        global_db->execute("TRUNCATE rooms_all RESTART IDENTITY CASCADE");
        global_db->execute("TRUNCATE lecture_room RESTART IDENTITY CASCADE");
        global_db->execute("TRUNCATE coworking_space RESTART IDENTITY CASCADE");
        global_db->execute("TRUNCATE private_office RESTART IDENTITY CASCADE");
        global_db->execute("TRUNCATE room_availability RESTART IDENTITY CASCADE"
        );
        global_db->execute("TRUNCATE room_booking RESTART IDENTITY CASCADE");
    }
};

#endif  // TEST_DB_FIXTURE_HPP_
