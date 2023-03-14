export module Utils.Throttle;

import <string_view>;
import <thread>;
import <map>;

import Utils.Time;

std::map<std::string_view, unsigned long> lastTime;

export void throttleFor(int milliseconds, std::string_view name) {
    auto currentTime = getTickCount();

    if (lastTime.find(name) != lastTime.end()) {
        auto last = lastTime[name];
        if (currentTime - last < milliseconds) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(milliseconds - (currentTime - last)));
        }
    }

    lastTime[name] = getTickCount();
}
