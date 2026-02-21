/***
 * Database users module (source).
 ***/

#include "../include/db_users.hpp"
#include <iostream>

std::ostream &roomsched::db::operator<<(
    std::ostream &os,
    const roomsched::db::user &new_user
) {
    os << new_user.id << "\t" << new_user.username << "\t\t"
       << new_user.created_at << "\t" << new_user.last_login << std::endl;
    return os;
}
