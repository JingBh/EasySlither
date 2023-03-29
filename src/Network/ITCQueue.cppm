/**
 * A thread-safe queue for inter thread communication.
 */
export module Network.ITCQueue;

import <cstdint>;
import <mutex>;
import <queue>;
import <vector>;

import Network.ConnectionEnd;

export template<ConnectionEnd E>
class ITCQueue {
private:
    std::queue <std::vector<uint8_t>> queue;
    std::mutex mutex;

    static inline ITCQueue<E> *instance_{nullptr};
    static inline std::mutex mutex_;

public:
    void push(std::vector <uint8_t> &value) {
        std::lock_guard <std::mutex> lock(mutex);
        queue.push(value);
    }

    bool hasNext() {
        std::lock_guard <std::mutex> lock(mutex);
        return !queue.empty();
    }

    std::vector <uint8_t> pop() {
        std::unique_lock <std::mutex> lock(mutex);
        std::vector <uint8_t> value = queue.front();
        queue.pop();
        return value;
    }

    static ITCQueue<E> *getInstance() {
        mutex_.lock();
        if (instance_ == nullptr) {
            instance_ = new ITCQueue<E>();
        }
        mutex_.unlock();

        return instance_;
    }
};
