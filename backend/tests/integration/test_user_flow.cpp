/***
 * Test user flow (integration test).
 ***/

#include <gtest/gtest.h>
#include "fixtures/test_db_fixture.hpp"
#include "services/user_service.hpp"

namespace roomsched::db {

TEST_F(backend_db_test, UserFlow_Register_Login_GetAll) {
    user_service service(*global_db);

    service.register_user(
        "alexander_mail@gmail.com", "strong_12_pwd", "Alexander Glushchenko",
        "79211234567"
    );
    auto new_user = service.login("alexander_mail@gmail.com", "strong_12_pwd");
    ASSERT_TRUE(new_user.has_value());

    service.register_user(
        "diana_mail@gmail.com", "password_diana_123", "Diana Kusakina",
        "79211111111"
    );
    auto new_user1 =
        service.login("diana_mail@gmail.com", "password_diana_123");
    ASSERT_TRUE(new_user1.has_value());

    service.register_user(
        "varya_mail@gmail.com", "password_varya_5638", "Varvara Lakhnova",
        "79212222222"
    );
    auto new_user2 =
        service.login("varya_mail@gmail.com", "password_varya_5638");
    ASSERT_TRUE(new_user2.has_value());

    auto dup_fullname = service.register_user(
        "new_varya_mail@gmail.com", "password_varya_5638", "Varvara Lakhnova",
        "79213333333"
    );
    EXPECT_FALSE(dup_fullname.success);
    EXPECT_EQ(dup_fullname.error, register_error::fullname_exists);

    auto all_users = service.get_all_users();
    ASSERT_EQ(all_users.size(), 3);
    EXPECT_EQ(all_users[0].id, new_user->id);
    EXPECT_EQ(all_users[1].full_name, new_user1->full_name);
    EXPECT_EQ(all_users[2].phone, new_user2->phone);
}

}  // namespace roomsched::db
