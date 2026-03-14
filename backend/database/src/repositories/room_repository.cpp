/***
 * Database room repository module (source).
 ***/

#include "repositories/room_repository.hpp"
#include <exception>
#include <iostream>

roomsched::db::room_repository::room_repository(database &db_) : db(db_) {
}

int roomsched::db::room_repository::create_room(const room &new_room) {
    try {
        const auto result = db.query(
            "INSERT INTO rooms_all(room_number, building, floor, total_area, "
            "description, type) VALUES($1, $2, $3, $4, $5, $6) RETURNING id",
            new_room.room_number, new_room.building, new_room.floor,
            new_room.total_area, new_room.description,
            convert_roomtype_to_string(new_room.type)
        );
        int current_room_id = result[0]["id"].as<int>();

        if (new_room.is_lecture_room()) {
            create_lecture_room(current_room_id, new_room);
        } else if (new_room.is_coworking_room()) {
            create_coworking_space(current_room_id, new_room);
        } else if (new_room.is_office_room()) {
            create_private_office(current_room_id, new_room);
        } else {
            throw std::runtime_error("Unknown room type");
        }

        std::cout << "Room with index '" << current_room_id << "' was created!"
                  << std::endl;
        return current_room_id;
    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in create_room]: " << e.what() << std::endl
                  << "Query: " << e.query() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in create_room]: " << e.what() << std::endl;
    }
    return -1;
}

roomsched::db::room roomsched::db::room_repository::get_room_by_id(int id) {
    room current_room;

    try {
        const auto result_room = db.query(
            "SELECT r.room_number, r.building, r.floor, r.total_area, "
            "r.description, r.type, lr.has_projector, lr.has_whiteboard, "
            "lr.capacity, cs.total_capacity, cs.has_wifi, cs.has_printers, "
            "po.number_of_chairs, po.has_phone FROM rooms_all r "
            "LEFT JOIN lecture_room lr ON r.id = lr.room_id "
            "LEFT JOIN coworking_space cs ON r.id = cs.room_id "
            "LEFT JOIN private_office po ON r.id = po.room_id "
            "WHERE r.id = $1 ",
            id
        );
        const auto row = result_room[0];

        current_room.id = id;
        current_room.room_number = row["room_number"].as<std::string>();
        current_room.building = row["building"].as<std::string>();
        current_room.floor = row["floor"].as<int>();
        current_room.total_area = row["total_area"].as<double>();
        current_room.description = row["description"].as<std::string>();
        current_room.type =
            convert_string_to_roomtype(row["type"].as<std::string>());

        if (current_room.is_lecture_room()) {
            current_room.has_projector = row["has_projector"].as<bool>();
            current_room.has_whiteboard = row["has_whiteboard"].as<bool>();
            current_room.capacity = row["capacity"].as<int>();
        } else if (current_room.is_coworking_room()) {
            current_room.total_capacity = row["total_capacity"].as<int>();
            current_room.has_wifi = row["has_wifi"].as<bool>();
            current_room.has_printers = row["has_printers"].as<bool>();
        } else if (current_room.is_office_room()) {
            current_room.number_of_chairs = row["number_of_chairs"].as<int>();
            current_room.has_phone = row["has_phone"].as<bool>();
        } else {
            throw std::runtime_error("Unknown room type");
        }

    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in get_room_by_id]: " << e.what() << std::endl
                  << "Query: " << e.query() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in get_room_by_id]: " << e.what()
                  << std::endl;
    }

    return current_room;
}

std::vector<roomsched::db::room> roomsched::db::room_repository::get_all_rooms(
) {
    std::vector<room> all_rooms;

    try {
        const auto result = db.query(
            "SELECT id, room_number, building, floor, total_area, description "
            "FROM rooms_all ORDER BY id"
        );

        for (const auto &row : result) {
            room current_room;
            current_room.id = row["id"].as<int>();
            current_room.room_number = row["room_number"].as<std::string>();
            current_room.building = row["building"].as<std::string>();
            current_room.floor = row["floor"].as<int>();
            current_room.total_area = row["total_area"].as<double>();
            current_room.description = row["description"].as<std::string>();
            current_room.type =
                convert_string_to_roomtype(row["type"].as<std::string>());

            all_rooms.push_back(current_room);
        }
    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in get_all_rooms]: " << e.what() << std::endl
                  << "Query: " << e.query() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in get_all_rooms]: " << e.what()
                  << std::endl;
    }
    return all_rooms;
}

void roomsched::db::room_repository::create_lecture_room(
    int room_id,
    const room &r
) {
    try {
        db.execute(
            "INSERT INTO lecture_room(room_id, has_projector, has_whiteboard, "
            "capacity) VALUES($1, $2, $3, $4)",
            room_id, r.has_projector.value_or(false),
            r.has_whiteboard.value_or(true), r.capacity.value_or(100)
        );
        std::cout << "lecture room was created!" << std::endl;
    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in create_lecture_room]: " << e.what()
                  << std::endl
                  << "Query: " << e.query() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in create_lecture_room]: " << e.what()
                  << std::endl;
    }
}

void roomsched::db::room_repository::create_coworking_space(
    int room_id,
    const room &r
) {
    try {
        db.execute(
            "INSERT INTO coworking_space(room_id, total_capacity, has_wifi, "
            "has_printers) VALUES($1, $2, $3, $4)",
            room_id, r.total_capacity.value_or(50), r.has_wifi.value_or(true),
            r.has_printers.value_or(false)
        );
        std::cout << "coworking space was created!" << std::endl;
    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in create_coworking_space]: " << e.what()
                  << std::endl
                  << "Query: " << e.query() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in create_coworking_space]: " << e.what()
                  << std::endl;
    }
}

void roomsched::db::room_repository::create_private_office(
    int room_id,
    const room &r
) {
    try {
        db.execute(
            "INSERT INTO private_office(room_id, number_of_chairs, has_phone) "
            "VALUES($1, $2, $3)",
            room_id, r.number_of_chairs.value_or(2), r.has_phone.value_or(false)
        );
        std::cout << "private office was created!" << std::endl;
    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in create_private_office]: " << e.what()
                  << std::endl
                  << "Query: " << e.query() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in create_private_office]: " << e.what()
                  << std::endl;
    }
}

roomsched::db::room_type
roomsched::db::room_repository::convert_string_to_roomtype(
    const std::string &str
) {
    if (str == "LECTURE" || str == "lecture") {
        return room_type::LECTURE;
    } else if (str == "COWORKING" || str == "coworking") {
        return room_type::COWORKING;
    } else if (str == "PRIVATE_OFFICE" || str == "private_office") {
        return room_type::PRIVATE_OFFICE;
    } else {
        throw std::runtime_error("Unknown room type!");
    }
}

std::string roomsched::db::room_repository::convert_roomtype_to_string(
    room_type type
) {
    if (type == room_type::LECTURE) {
        return "lecture";
    } else if (type == room_type::COWORKING) {
        return "coworking";
    } else if (type == room_type::PRIVATE_OFFICE) {
        return "private_office";
    } else {
        throw std::runtime_error("Unknown room type!");
    }
}
