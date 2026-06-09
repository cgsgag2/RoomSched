/***
 * Backend tests main module (source).
 ***/

#include <gtest/gtest.h>
#include <iostream>
#include <memory>
// Local includes
#include "core/db_core.hpp"

std::unique_ptr<roomsched::db::database> global_db;

class global_test_environment : public ::testing::Environment {
public:
    void SetUp() override {
        roomsched::db::db_config config(
            "roomsched_test", "rsched_user_test", "password_tests_roomsched",
            "postgres"
        );

        global_db = std::make_unique<roomsched::db::database>(config);
        if (!global_db->connect()) {
            throw std::runtime_error("Failed to connect to test DB");
        }
        std::cout << "[TEST ENV] Test database connected" << std::endl;

        // Reset DB
        global_db->execute("DROP SCHEMA public CASCADE");
        global_db->execute("CREATE SCHEMA public");
        global_db->execute("GRANT ALL ON SCHEMA public TO rsched_user_test");
        global_db->execute_sql_file("database/db_sql/02_create_tables.sql");

        std::cout << "[TEST ENV] Test schema initialized" << std::endl;
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
