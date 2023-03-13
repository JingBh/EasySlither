export module Utils.Frames;

import <chrono>;
import <string>;
import <thread>;
import <map>;

std::map<std::string, unsigned long> lastFrame;
std::map<std::string, unsigned long> lastTime;

const std::string defaultName = "default";

unsigned long GetTickCount() {
    static const auto start_time = std::chrono::steady_clock::now();
    auto current_time = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
    return static_cast<unsigned long>(ms);
}

export void sleepFor(int fps, const std::string &name = defaultName) {
    if (lastFrame.find(name) == lastFrame.end()) {
        lastFrame[name] = 0;
        lastTime[name] = GetTickCount();
    }

    unsigned long currentFrame = lastFrame[name];
    unsigned long currentTime = lastTime[name];

    unsigned long frameTime = 1000 / fps;
    unsigned long time = GetTickCount();

    if (time - currentTime < frameTime) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(frameTime - (time - currentTime)));
    }

    lastFrame[name] = currentFrame + 1;
    lastTime[name] = GetTickCount();
}
