/***
 * Test user repository module (unit test).
 ***/

#include <gtest/gtest.h>
#include "fixtures/test_db_fixture.hpp"
#include "repositories/user_repository.hpp"

namespace roomsched::db {

TEST_F(backend_db_test, UserRepo_CreateUser_Success) {
    user_repository repo(*global_db);

    user u;
    u.email = "test@gmail.com";
    u.password_hash = "pwd_test";
    u.full_name = "Alexander";
    u.phone = "+79211234567";
    u.role = "user";

    EXPECT_TRUE(repo.create_user(u));
}

TEST_F(backend_db_test, UserRepo_CreateUser_Dublicate) {
    user_repository repo(*global_db);

    user u;
    u.email = "test@gmail.com";
    u.password_hash = "pwd_test";
    u.full_name = "Alexander";
    u.phone = "+79211234567";
    u.role = "user";

    repo.create_user(u);
    EXPECT_FALSE(repo.create_user(u));
}

TEST_F(backend_db_test, UserRepo_GetUserByEmail_Found) {
    user_repository repo(*global_db);

    user u;
    u.email = "test_email@gmail.com";
    u.password_hash = "pwd_test";
    u.full_name = "Alexander";
    u.phone = "+79211234567";
    u.role = "user";

    repo.create_user(u);
    auto result = repo.get_user_by_email("test_email@gmail.com");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->email, "test_email@gmail.com");
}

TEST_F(backend_db_test, UserRepo_GetUserByEmail_NotFound_Empty) {
    user_repository repo(*global_db);

    auto result = repo.get_user_by_email("testincorrect_email@gmail.com");

    EXPECT_FALSE(result.has_value());
}

TEST_F(backend_db_test, UserRepo_GetUserByEmail_NotFoundIncorrect) {
    user_repository repo(*global_db);

    user u;
    u.email = "test_email@gmail.com";
    u.password_hash = "pwd_test";
    u.full_name = "Alexander";
    u.phone = "+79211234567";
    u.role = "user";

    repo.create_user(u);
    auto result = repo.get_user_by_email("testincorrect_email@gmail.com");

    EXPECT_FALSE(result.has_value())
        << "No user with testincorrect_email@gmail.com email";
}

}  // namespace roomsched::db
