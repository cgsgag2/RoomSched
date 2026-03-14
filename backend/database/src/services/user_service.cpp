/***
 * Database user service module (source).
 ***/

#include "services/user_service.hpp"
#include <iostream>

roomsched::db::user_service::user_service(database &db_) : repo(db_) {
}

void roomsched::db::user_service::register_user(const user &new_user) {
    if (repo.create_user(new_user)) {
        std::cout << "User " << new_user.username << " was created!"
                  << std::endl;
    }
}

roomsched::db::user roomsched::db::user_service::get_user_by_id(int id) {
    return repo.get_user_by_id(id);
}

std::vector<roomsched::db::user> roomsched::db::user_service::get_all_users() {
    return repo.get_all_users();
}
