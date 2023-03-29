export module Utils.BoundBox;

import <cmath>;

export class HasRoundBoundBox {
private:
    double boundBoxX, boundBoxY;
    double boundBoxRadius;
public:
    HasRoundBoundBox(const double x, const double y, const double radius)
        : boundBoxX{x}, boundBoxY{y}, boundBoxRadius{radius} {}

    [[nodiscard]] bool isIntersect(const HasRoundBoundBox &other) const {
        const auto dx = this->boundBoxX - other.boundBoxX;
        const auto dy = this->boundBoxY - other.boundBoxY;
        const auto distance = std::sqrt(dx * dx + dy * dy);
        return distance <= this->boundBoxRadius + other.boundBoxRadius;
    }
};
