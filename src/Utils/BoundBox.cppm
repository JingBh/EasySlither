export module Utils.BoundBox;

import <cmath>;

export class HasRoundBoundBox {
public:
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

    [[nodiscard]] bool isInclude(const double x, const double y) const {
        const auto dx = this->boundBoxX - x;
        const auto dy = this->boundBoxY - y;
        const auto distance = std::sqrt(dx * dx + dy * dy);
        return distance <= this->boundBoxRadius;
    }
};

export class HasRectBoundBox {
public:
    double boundBoxX1, boundBoxY1, boundBoxX2, boundBoxY2;

public:
    HasRectBoundBox(const double x1, const double y1, const double x2, const double y2)
        : boundBoxX1{x1}, boundBoxY1{y1}, boundBoxX2{x2}, boundBoxY2{y2} {}

    [[nodiscard]] bool isIntersect(const HasRectBoundBox &other) const {
        return this->boundBoxX1 <= other.boundBoxX2 && this->boundBoxX2 >= other.boundBoxX1
               && this->boundBoxY1 <= other.boundBoxY2 && this->boundBoxY2 >= other.boundBoxY1;
    }

    [[nodiscard]] bool isIntersect(const HasRoundBoundBox &other) const {
        double dx = std::fmax(this->boundBoxX1 - other.boundBoxX, 0);
        dx = std::max(dx, other.boundBoxX - this->boundBoxX2);
        double dy = std::fmax(this->boundBoxY1 - other.boundBoxY, 0.0f);
        dy = std::max(dy, other.boundBoxY - this->boundBoxY2);
        double distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= other.boundBoxRadius) {
            return true;
        }

        if (other.boundBoxX + other.boundBoxRadius > this->boundBoxX1 &&
            other.boundBoxX - other.boundBoxRadius < this->boundBoxX2 &&
            other.boundBoxY + other.boundBoxRadius < this->boundBoxY1 &&
            other.boundBoxY - other.boundBoxRadius > this->boundBoxY2) {
            return true;
        }

        return false;
    }

    [[nodiscard]] bool isInclude(const double x, const double y) const {
        return this->boundBoxX1 <= x && this->boundBoxX2 >= x
               && this->boundBoxY1 <= y && this->boundBoxY2 >= y;
    }
};
