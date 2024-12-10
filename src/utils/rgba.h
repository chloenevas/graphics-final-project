#pragma once

#include <cstdint>
#include <algorithm> // For std::clamp

struct RGBA {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t a = 255;

    // Overload the += operator
    RGBA& operator+=(const RGBA& other) {
        r = std::clamp(static_cast<int>(r) + static_cast<int>(other.r), 0, 255);
        g = std::clamp(static_cast<int>(g) + static_cast<int>(other.g), 0, 255);
        b = std::clamp(static_cast<int>(b) + static_cast<int>(other.b), 0, 255);
        a = std::clamp(static_cast<int>(a) + static_cast<int>(other.a), 0, 255);
        return *this;
    }

    // Overload the /= operator
    RGBA& operator/=(int divisor) {
        if (divisor == 0) {
            throw std::runtime_error("Division by zero");
        }
        r = static_cast<std::uint8_t>(std::clamp(r / divisor, 0, 255));
        g = static_cast<std::uint8_t>(std::clamp(g / divisor, 0, 255));
        b = static_cast<std::uint8_t>(std::clamp(b / divisor, 0, 255));
        a = static_cast<std::uint8_t>(std::clamp(a / divisor, 0, 255));
        return *this;
    }
};

// // Optional: Free function for `+` operator
// RGBA operator+(const RGBA& lhs, const RGBA& rhs) {
//     RGBA result = lhs;
//     result += rhs;
//     return result;
// }

// // Optional: Free function for `/` operator
// RGBA operator/(const RGBA& lhs, int divisor) {
//     RGBA result = lhs;
//     result /= divisor;
//     return result;
// }
