/***
 * Test user service module (unit test).
 ***/

#include <gtest/gtest.h>
#include "fixtures/test_db_fixture.hpp"
#include "services/user_service.hpp"

namespace roomsched::db {

TEST_F(backend_db_test, UserService_RegisterUser_success) {
    user_service service(*global_db);

    EXPECT_TRUE(service.register_user(
        "test@gmail.com", "pwd_test12", "Alexander", "79211234567"
    ));
}

TEST_F(backend_db_test, UserService_RegisterUser_WeakPassword) {
    user_service service(*global_db);

    EXPECT_FALSE(service.register_user(
        "sanya@gmail.com", "pwd", "Alexander", "79211234567"
    ));
}

TEST_F(backend_db_test, UserService_RegisterUser_DublicateEmail) {
    user_service service(*global_db);

    service.register_user(
        "test@gmail.com", "pwd_test12", "Alexander", "79211234567"
    );
    EXPECT_FALSE(service.register_user(
        "test@gmail.com", "pwd_test1234", "Diana", "79210000000"
    ));
}

TEST_F(backend_db_test, UserService_RegisterUser_DublicatePhone) {
    user_service service(*global_db);

    service.register_user(
        "test_sanya@gmail.com", "s_pwd_test12", "Alexander", "79211234567"
    );
    EXPECT_FALSE(service.register_user(
        "test_diana@gmail.com", "strong_pwd_1234", "Diana", "79211234567"
    ));
}

TEST_F(backend_db_test, UserService_RegisterUser_DublicateFullname) {
    user_service service(*global_db);

    service.register_user(
        "sanya@gmail.com", "pwd_test12", "Alexander", "79211234567"
    );
    EXPECT_FALSE(service.register_user(
        "varya@gmail.com", "strong_pwd_1234", "Alexander", "79210000000"
    ));
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
