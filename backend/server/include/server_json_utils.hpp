/***
 * Server json utilities module (header).
 ***/

#ifndef SERVER_JSON_UTILS_HPP_
#define SERVER_JSON_UTILS_HPP_

#include <crow/http_response.h>
#include <crow/json.h>
#include <string>
#include <vector>

// project namespace, server module namespace
namespace roomsched::server::json_utils {
crow::response
error_response(const std::string &message, int status_code = 400);

bool validate_fields(
    const crow::json::rvalue &json,
    const std::vector<std::string> &fields
);

crow::response
success_response(const std::string &message, crow::json::wvalue data = {});
}  // namespace roomsched::server::json_utils

#endif  // SERVER_JSON_UTILS_HPP_
