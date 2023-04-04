export module Utils.Time;

import <chrono>;

export unsigned long long getTickCount() {
    static const auto start_time = std::chrono::steady_clock::now();
    auto current_time = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
    return static_cast<unsigned long long>(ms);
}
