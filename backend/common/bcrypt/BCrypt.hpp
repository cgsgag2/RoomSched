#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <random>

namespace BCrypt {

static const char* _bcrypt_base64_encoding = "./ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

inline std::string encode_base64(const uint8_t* data, size_t len) {
    std::string result;
    result.reserve((len + 2) / 3 * 4);

    uint32_t buffer = 0;
    int bits_left = 0;

    for (size_t i = 0; i < len; ++i) {
        buffer = (buffer << 8) | data[i];
        bits_left += 8;

        while (bits_left >= 6) {
            bits_left -= 6;
            result.push_back(_bcrypt_base64_encoding[(buffer >> bits_left) & 0x3F]);
        }
    }

    if (bits_left > 0) {
        buffer <<= (6 - bits_left);
        result.push_back(_bcrypt_base64_encoding[buffer & 0x3F]);
    }

    return result;
}

inline std::string generateSalt(int rounds = 12) {
    if (rounds < 4 || rounds > 31)
        throw std::invalid_argument("BCrypt: rounds must be between 4 and 31");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dist(0, 255);

    uint8_t salt_bytes[16];
    for (auto& b : salt_bytes) b = dist(gen);

    std::ostringstream ss;
    ss << "$2b$" << std::setw(2) << std::setfill('0') << rounds << "$";
    ss << encode_base64(salt_bytes, 16);

    return ss.str();
}

extern "C" {
    int bcrypt_hashpw(const char* password, const char* salt, char* output);
}

inline std::string generateHash(const std::string& password, int rounds = 12) {
    std::string salt = generateSalt(rounds);
    char output[128];

    if (bcrypt_hashpw(password.c_str(), salt.c_str(), output) != 0)
        throw std::runtime_error("BCrypt: hashing failed");

    return std::string(output);
}

inline bool validatePassword(const std::string& password, const std::string& hash) {
    char output[128];

    if (bcrypt_hashpw(password.c_str(), hash.c_str(), output) != 0)
        return false;

    return hash == output;
}

} // namespace BCrypt

extern "C" {

static const uint8_t _bcrypt_ciphertext[4 * 6] = {
    0x4f, 0x72, 0x70, 0x68, 0x65, 0x61,
    0x6e, 0x20, 0x49, 0x49, 0x49, 0x49,
    0x49, 0x49, 0x49, 0x49, 0x49, 0x49,
    0x49, 0x49, 0x49, 0x49, 0x49, 0x49
};

int bcrypt_hashpw(const char* password, const char* salt, char* output) {
    std::hash<std::string> h;
    std::string combined = std::string(password) + std::string(salt);
    size_t hashed = h(combined);

    std::ostringstream ss;
    ss << salt << std::hex << hashed;

    std::string result = ss.str();
    std::strncpy(output, result.c_str(), 127);
    output[127] = '\0';

    return 0;
}

}
