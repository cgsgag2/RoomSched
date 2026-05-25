/***
 * Test user service module (unit test).
 ***/

#include <gtest/gtest.h>
#include "fixtures/test_db_fixture.hpp"
#include "services/user_service.hpp"

namespace roomsched::db {

TEST_F(backend_db_test, UserService_RegisterUser_success) {
    user_service service(*global_db);

    auto ok = service.register_user(
        "test@gmail.com", "pwd_test12", "Alexander", "79211234567"
    );
    EXPECT_TRUE(ok.success);
    EXPECT_EQ(ok.error, register_error::none);
}

TEST_F(backend_db_test, UserService_RegisterUser_WeakPassword) {
    user_service service(*global_db);

    auto weak = service.register_user(
        "sanya@gmail.com", "pwd", "Alexander", "79211234567"
    );
    EXPECT_FALSE(weak.success);
    EXPECT_EQ(weak.error, register_error::weak_password);
}

TEST_F(backend_db_test, UserService_RegisterUser_DublicateEmail) {
    user_service service(*global_db);

    service.register_user(
        "test@gmail.com", "pwd_test12", "Alexander", "79211234567"
    );
    auto dup_email = service.register_user(
        "test@gmail.com", "pwd_test1234", "Diana", "79210000000"
    );
    EXPECT_FALSE(dup_email.success);
    EXPECT_EQ(dup_email.error, register_error::email_exists);
}

TEST_F(backend_db_test, UserService_RegisterUser_DublicatePhone) {
    user_service service(*global_db);

    service.register_user(
        "test_sanya@gmail.com", "s_pwd_test12", "Alexander", "79211234567"
    );
    auto dup_phone = service.register_user(
        "test_diana@gmail.com", "strong_pwd_1234", "Diana", "79211234567"
    );
    EXPECT_FALSE(dup_phone.success);
    EXPECT_EQ(dup_phone.error, register_error::phone_exists);
}

TEST_F(backend_db_test, UserService_RegisterUser_DublicateFullname) {
    user_service service(*global_db);

    service.register_user(
        "sanya@gmail.com", "pwd_test12", "Alexander", "79211234567"
    );
    auto dup_fullname = service.register_user(
        "varya@gmail.com", "strong_pwd_1234", "Alexander", "79210000000"
    );
    EXPECT_FALSE(dup_fullname.success);
    EXPECT_EQ(dup_fullname.error, register_error::fullname_exists);
}

TEST_F(backend_db_test, UserService_Login_Success) {
    user_service service(*global_db);

    service.register_user(
        "sanya@gmail.com", "strong_pwd_30120", "Alexander", "79210000000"
    );
    auto result = service.login("sanya@gmail.com", "strong_pwd_30120");
    EXPECT_TRUE(result.has_value());
}

TEST_F(backend_db_test, UserService_Login_WrongPassword) {
    user_service service(*global_db);

    service.register_user(
        "sanya@gmail.com", "strong_pwd_30120", "Alexander", "79210000000"
    );
    auto result = service.login("sanya@gmail.com", "wrong_pwd_30");
    EXPECT_FALSE(result.has_value());
}

}  // namespace roomsched::db
