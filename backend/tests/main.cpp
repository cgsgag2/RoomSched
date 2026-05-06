/***
 * Backend tests main module (source).
 ***/

#include <gtest/gtest.h>
#include <iostream>
#include <memory>
// Local libs
#include "core/db_core.hpp"

std::unique_ptr<roomsched::db::database> global_db;

class global_test_environment : public ::testing::Environment {
public:
    void SetUp() override {
        roomsched::db::db_config config(
            "roomsched_test", "rsched_user_test", "password_tests_roomsched",
            "localhost"
        );

        global_db = std::make_unique<roomsched::db::database>(config);
        if (!global_db->connect()) {
            throw std::runtime_error("Failed to connect to test DB");
        }
        std::cout << "[TEST ENV] Test database connected" << std::endl;
    }

    void TearDown() override {
        if (global_db) {
            global_db->disconnect();
            global_db.reset();
        }

        std::cout << "[TEST ENV] Test DB disconnected" << std::endl;
    }
};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new global_test_environment());

    return RUN_ALL_TESTS();
}
