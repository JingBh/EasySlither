export module Utils.Throttle;

import <string_view>;
import <thread>;
import <map>;

import Utils.Time;

std::map<std::string_view, unsigned long long> lastTime;

export bool throttleFor(int milliseconds, std::string_view name) {
    auto currentTime = getTickCount();

    if (lastTime.find(name) != lastTime.end()) {
        auto last = lastTime[name];

        if (currentTime - last < milliseconds) {
            return false;
        }
    }

    lastTime[name] = getTickCount();

    return true;
}
