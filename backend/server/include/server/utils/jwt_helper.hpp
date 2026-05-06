#ifndef JWT_HELPER_HPP
#define JWT_HELPER_HPP

#include <jwt-cpp/jwt.h>
#include <crow.h> 
#include <string>
#include <chrono>
#include <optional>

namespace roomsched::server {

class JwtHelper {
private:
    static inline const std::string SECRET_KEY = "roomsched_private_super_secure_key_2026";
    static inline const std::string ISSUER = "RoomSchedServer";

public:
    struct TokenData {
        int userId;
        std::string email;
    };

    static std::string createToken(int userId, const std::string& email) {
        auto token = jwt::create()
            .set_issuer(ISSUER)
            .set_type("JWS")
            .set_payload_claim("user_id", jwt::claim(std::to_string(userId)))
            .set_payload_claim("email", jwt::claim(email))
            .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours{24})
            .sign(jwt::algorithm::hs256{SECRET_KEY});
        return token;
    }

    static std::optional<TokenData> verifyToken(const std::string& tokenStr) {
        try {
            auto decoded = jwt::decode(tokenStr);
            
            auto verifier = jwt::verify()
                .allow_issuer(ISSUER)
                .with_algorithm(jwt::algorithm::hs256{SECRET_KEY});

            verifier.verify(decoded);

            TokenData data;
            data.userId = std::stoi(decoded.get_payload_claim("user_id").as_string());
            data.email = decoded.get_payload_claim("email").as_string();
            
            return data;
        } catch (const std::exception& e) {
            return std::nullopt; 
        }
    }
};

inline std::optional<JwtHelper::TokenData> getAuthData(const crow::request& req) {
    std::string authHeader = req.get_header_value("Authorization");
    if (authHeader.empty() || authHeader.rfind("Bearer ", 0) != 0) {
        return std::nullopt; 
    }
    std::string token = authHeader.substr(7);
    return JwtHelper::verifyToken(token);
}

} // namespace roomsched::server

#endif // JWT_HELPER_HPP