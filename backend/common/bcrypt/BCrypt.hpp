#pragma once
#include <string>
#include <stdexcept>
#include <crypt.h>

namespace BCrypt {

inline std::string generateHash(const std::string& password, int rounds = 12) {
    char salt[64];
    // Генерируем соль в формате $2b$ (BCrypt)
    if (crypt_gensalt_rn("$2b$", rounds, nullptr, 0, salt, sizeof(salt)) == nullptr) {
        throw std::runtime_error("BCrypt: salt generation failed");
    }

    struct crypt_data data;
    data.initialized = 0;
    char* hashed = crypt_r(password.c_str(), salt, &data);
    
    if (hashed == nullptr)
        throw std::runtime_error("BCrypt: hashing failed");

    return std::string(hashed);
}

inline bool validatePassword(const std::string& password, const std::string& hash) {
    struct crypt_data data;
    data.initialized = 0;
    char* hashed = crypt_r(password.c_str(), hash.c_str(), &data);
    
    if (hashed == nullptr) return false;
    return hash == hashed;
}

} // namespace BCrypt