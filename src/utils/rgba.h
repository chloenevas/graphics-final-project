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
        r += other.r;
        g += other.g;
        b += other.b;
        return *this;
    }

    // Overload the /= operator
    RGBA& operator/=(int divisor) {
        if (divisor == 0) {
            throw std::runtime_error("Division by zero");
        }
        r /= divisor;
        g /= divisor;
        b /= divisor;
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
