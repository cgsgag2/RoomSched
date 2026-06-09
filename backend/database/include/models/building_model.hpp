/***
 * Database building model module (header).
 ***/

#ifndef BUILDING_MODEL_HPP_
#define BUILDING_MODEL_HPP_

#include <string>

// project namespace, database module namespace
namespace roomsched::db {
class building {
public:
    int id;
    std::string name;
    std::string address;
};
}  // namespace roomsched::db

#endif  // BUILDING_MODEL_HPP_
