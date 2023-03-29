export module Utils.Random;

import <random>;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> intDis;
std::uniform_real_distribution<double> doubleDis;

export int randomInt(const int min, const int max) {
    intDis = std::uniform_int_distribution<int>(min, max);
    return intDis(gen);
}

export int randomInt(const int base) {
    return randomInt(0, base - 1);
}

export double randomDouble(const double min, const double max) {
    doubleDis = std::uniform_real_distribution<double>(min, max);
    return doubleDis(gen);
}
