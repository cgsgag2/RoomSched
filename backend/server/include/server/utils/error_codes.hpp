/***
 * Server error codes (header).
 ***/

#ifndef ERROR_CODES_HPP_
#define ERROR_CODES_HPP_

namespace roomsched::server::error_codes {
constexpr const char *kInvalidJson = "INVALID_JSON";
constexpr const char *kMissingFields = "MISSING_FIELDS";
constexpr const char *kInvalidEmail = "INVALID_EMAIL";
constexpr const char *kWeakPassword = "WEAK_PASSWORD";
constexpr const char *kEmailExists = "EMAIL_EXISTS";
constexpr const char *kPhoneExists = "PHONE_EXISTS";
constexpr const char *kFullnameExists = "FULLNAME_EXISTS";
constexpr const char *kRegistrationFailed = "REGISTRATION_FAILED";
constexpr const char *kInvalidCredentials = "INVALID_CREDENTIALS";
constexpr const char *kBadRequest = "BAD_REQUEST";
constexpr const char *kBookingConflict = "BOOKING_CONFLICT";
constexpr const char *kInvalidTimeRange = "INVALID_TIME_RANGE";
constexpr const char *kBookingNotFound = "BOOKING_NOT_FOUND";
constexpr const char *kRoomNotFound = "ROOM_NOT_FOUND";
constexpr const char *kDatabaseError = "DATABASE_ERROR";
constexpr const char *kInternalError = "INTERNAL_ERROR";
constexpr const char *kTelegramError = "TELEGRAM_ERROR";
}  // namespace roomsched::server::error_codes

#endif  // ERROR_CODES_HPP_
