/***
 * Database telegram repository module (source).
 ***/

#include "repositories/telegram_repository.hpp"
#include <iostream>

namespace roomsched::db {

telegram_repository::telegram_repository(database &db_) : db(db_) {
}

bool telegram_repository::create_link(const telegram_link &tg) {
    try {
        db.execute(
            "INSERT INTO telegram_links(code, user_id) VALUES ($1, $2)",
            tg.code, tg.user_id
        );
        return true;
    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in create_link]: " << e.what() << std::endl
                  << "Query: " << e.query() << std::endl;
        return false;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in create_link]: " << e.what() << std::endl;
        return false;
    }
}

bool telegram_repository::delete_link(const std::string &code) {
    try {
        db.execute("DELETE FROM telegram_links WHERE code = $1", code);

        return true;
    } catch (const pqxx::sql_error &e) {
        std::cerr << "[SQL ERROR in delete_link]: " << e.what() << std::endl
                  << "Query: " << e.query() << std::endl;
        return false;
    } catch (const std::exception &e) {
        std::cerr << "[DB EXCEPTION in delete_link]: " << e.what() << std::endl;
        return false;
    }
}

std::optional<int> telegram_repository::get_user_id_by_code(
    const std::string &code
) {
    try {
        auto result = db.query(
            "SELECT user_id FROM telegram_links WHERE code = $1", code
        );

        if (result.empty()) {
            return std::nullopt;
        }

        return result[0]["user_id"].as<int>();
    } catch (const std::exception &e) {
        std::cerr << "[DB ERROR in get_user_id_by_code]: " << e.what()
                  << std::endl;
        return std::nullopt;
    }
}

}  // namespace roomsched::db
