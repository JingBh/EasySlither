export module Utils.Angle;

import <cmath>;
import <numbers>;

export double normalizeAngle(const double angle) {
    auto result = std::fmod(angle, 2 * std::numbers::pi);

    if (result < -std::numbers::pi) {
        result += std::numbers::pi * 2;
    }

    if (result > std::numbers::pi) {
        result -= std::numbers::pi * 2;
    }

    return result;
}

export double rotateAngle(const double angle, const double delta) {
    return normalizeAngle(angle + delta);
}

export double reverseAngle(const double angle) {
    return rotateAngle(angle, std::numbers::pi);
}
