/***
 * Database telegram link model module (header).
 ***/

#ifndef TELEGRAM_LINK_MODEL_HPP_
#define TELEGRAM_LINK_MODEL_HPP_

#include <cstdint>
#include <string>

// project namespace, database module namespace
namespace roomsched::db {
class telegram_link {
public:
    std::string code;
    int user_id;
    std::string created_at;
};

}  // namespace roomsched::db

#endif  // TELEGRAM_LINK_MODEL_HPP_
