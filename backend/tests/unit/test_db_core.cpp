/***
 * Test database core module (unit test).
 ***/

#include <gtest/gtest.h>
#include "fixtures/test_db_fixture.hpp"

namespace roomsched::db {

TEST_F(backend_db_test, Database_Connect_Success) {
    ASSERT_NE(global_db, nullptr);
}

TEST_F(backend_db_test, Database_ExecuteQuery_Success) {
    EXPECT_NO_THROW(global_db->execute(
        "INSERT INTO users (email, password_hash, full_name, phone, role) "
        "VALUES ($1, $2, $3, $4, $5)",
        "alex_test@gmail.com", "pwd_strong128", "Alexander", "+70001112233",
        "user"
    ));
}

TEST_F(backend_db_test, Database_InvalidQuery_Fail) {
    EXPECT_ANY_THROW(global_db->execute("INVALID SQL QUERY"));
}

TEST_F(backend_db_test, Database_Select_Empty) {
    auto result = global_db->query("SELECT * FROM users;");
    EXPECT_EQ(result.size(), 0);
}

}  // namespace roomsched::db
