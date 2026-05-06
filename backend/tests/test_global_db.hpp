/***
 * Test global database module (header).
 ***/

#ifndef TEST_GLOBAL_DB_HPP_
#define TEST_GLOBAL_DB_HPP_

#include <memory>
#include "core/db_core.hpp"

extern std::unique_ptr<roomsched::db::database> global_db;

#endif  // TEST_GLOBAL_DB_HPP_
