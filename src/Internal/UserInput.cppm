export module Internal.UserInput;

import <cmath>;
import <numbers>;
import <mutex>;

import Screen.ScaleFactor;
import ThirdParty;
import Utils.Observer;
import Utils.Throttle;

export class SubjectInputDirection : public ISubject<int> {
private:
    static inline SubjectInputDirection *instance_{nullptr};
    static inline std::mutex mutex_;

public:
    void update() {
        int degree;

        /**
         * Xbox Controller input
         */
        windows::XinputState state;
        windows::createZeroMemory(&state, sizeof(windows::XinputState));
        const auto dwResult = windows::XinputGetState(&state);
        if (dwResult == windows::XINPUT_SUCCESS) {
            const auto leftThumbX = state.Gamepad.sThumbLX;
            const auto leftThumbY = state.Gamepad.sThumbLY;

            if ((leftThumbX > windows::XINPUT_LEFT_THUMB_DEADZONE ||
                 leftThumbX < -windows::XINPUT_LEFT_THUMB_DEADZONE) &&
                (leftThumbY > windows::XINPUT_LEFT_THUMB_DEADZONE ||
                 leftThumbY < -windows::XINPUT_LEFT_THUMB_DEADZONE)) {

                const auto leftThumbMagnitude = std::hypot(leftThumbX, leftThumbY);

                const auto normalizedLX = leftThumbX / leftThumbMagnitude;
                const auto normalizedLY = leftThumbY / leftThumbMagnitude;

                auto theta = std::atan2(normalizedLX, normalizedLY);
                if (theta < 0.0) {
                    theta += 2 * std::numbers::pi;
                }

                degree = static_cast<int>(theta * 180.0 / std::numbers::pi);
                return this->notify(degree);
            }

            if (state.Gamepad.wButtons & windows::XINPUT_DPAD_UP
                && state.Gamepad.wButtons & windows::XINPUT_DPAD_RIGHT) {
                degree = 45;
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_UP
                       && state.Gamepad.wButtons & windows::XINPUT_DPAD_LEFT) {
                degree = 315;
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_DOWN
                       && state.Gamepad.wButtons & windows::XINPUT_DPAD_RIGHT) {
                degree = 135;
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_DOWN
                       && state.Gamepad.wButtons & windows::XINPUT_DPAD_LEFT) {
                degree = 225;
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_UP) {
                degree = 0;
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_DOWN) {
                degree = 180;
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_RIGHT) {
                degree = 90;
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_LEFT) {
                degree = 270;
            } else {
                degree = 360; // flag indicates that shouldn't return
            }

            if (degree < 360) {
                return this->notify(degree);
            }
        }

        /**
         * Keyboard input
         */
        const bool isKeyUp = windows::isKeyDown(windows::VKEY_UP)
                             || windows::isKeyDown('W');
        const bool isKeyDown = windows::isKeyDown(windows::VKEY_DOWN)
                               || windows::isKeyDown('S');
        const bool isKeyLeft = windows::isKeyDown(windows::VKEY_LEFT)
                               || windows::isKeyDown('A');
        const bool isKeyRight = windows::isKeyDown(windows::VKEY_RIGHT)
                                || windows::isKeyDown('D');

        if (isKeyUp && isKeyRight) {
            degree = 45;
        } else if (isKeyUp && isKeyLeft) {
            degree = 315;
        } else if (isKeyDown && isKeyRight) {
            degree = 135;
        } else if (isKeyDown && isKeyLeft) {
            degree = 225;
        } else if (isKeyUp) {
            degree = 0;
        } else if (isKeyDown) {
            degree = 180;
        } else if (isKeyRight) {
            degree = 90;
        } else if (isKeyLeft) {
            degree = 270;
        } else {
            degree = 360; // flag indicates that shouldn't return
        }

        if (degree < 360) {
            return this->notify(degree);
        }
    }

    static SubjectInputDirection *getInstance() {
        mutex_.lock();
        if (instance_ == nullptr) {
            instance_ = new SubjectInputDirection();
        }
        mutex_.unlock();

        return instance_;
    }
};

export class ObservesInputDirection : public IObserver<int> {
public:
    ObservesInputDirection() {
        SubjectInputDirection::getInstance()->attach(this);
    }

    ~ObservesInputDirection() override {
        SubjectInputDirection::getInstance()->detach(this);
    }

    void update(const int &message) final {
        this->onInputDirection(message);
    }

    virtual void onInputDirection(int degree) = 0;
};

export class SubjectMouseMove : public ISubject<windows::Point> {
private:
    windows::Point lastPoint{-2, -2};

    static inline SubjectMouseMove *instance_{nullptr};
    static inline std::mutex mutex_;

public:
    void update(windows::WindowHandle handle) {
        auto point = windows::getMousePosition(handle);

        if (point.x != -1 && point.y != -1) {
            // High DPI scale
            point.x = static_cast<int>(static_cast<float>(point.x) / SCALE_FACTOR);
            point.y = static_cast<int>(static_cast<float>(point.y) / SCALE_FACTOR);

            if (point.x != lastPoint.x && point.y != lastPoint.y) {
                this->notify(point);
                lastPoint = point;
            }
        }
    }

    static SubjectMouseMove *getInstance() {
        mutex_.lock();
        if (instance_ == nullptr) {
            instance_ = new SubjectMouseMove();
        }
        mutex_.unlock();

        return instance_;
    }
};

export class ObservesMouseMove : public IObserver<windows::Point> {
public:
    ObservesMouseMove() {
        SubjectMouseMove::getInstance()->attach(this);
    }

    ~ObservesMouseMove() override {
        SubjectMouseMove::getInstance()->detach(this);
    }

    void update(const windows::Point &message) final {
        this->onMouseMove(message);
    }

    virtual void onMouseMove(const windows::Point &point) = 0;
};

export enum class KeyType {
    SWITCH,
    CONFIRM,
    CANCEL,
    INFO,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

export class SubjectKeyPress : public ISubject<KeyType> {
private:
    static inline SubjectKeyPress *instance_{nullptr};
    static inline std::mutex mutex_;

public:
    void update() {
        easyx::Message msg;

        while (easyx::peekMessage(&msg, easyx::MESSAGE_TYPE_MOUSE | easyx::MESSAGE_TYPE_KEY)) {
            switch (msg.message) {
                case easyx::MESSAGE_LBUTTONDOWN:
                    return this->notifyAndFlush(KeyType::CONFIRM);
                case easyx::MESSAGE_RBUTTONDOWN:
                    return this->notifyAndFlush(KeyType::INFO);
                case easyx::MESSAGE_KEYDOWN:
                    switch (msg.vkcode) {
                        case windows::VKEY_TAB:
                            return this->notifyAndFlush(KeyType::SWITCH);
                        case windows::VKEY_SPACE:
                        case windows::VKEY_ENTER:
                            return this->notifyAndFlush(KeyType::CONFIRM);
                        case windows::VKEY_ESCAPE:
                            return this->notifyAndFlush(KeyType::CANCEL);
                        case windows::VKEY_UP:
                            return this->notifyAndFlush(KeyType::UP);
                        case windows::VKEY_DOWN:
                            return this->notifyAndFlush(KeyType::DOWN);
                        case windows::VKEY_LEFT:
                            return this->notifyAndFlush(KeyType::LEFT);
                        case windows::VKEY_RIGHT:
                            return this->notifyAndFlush(KeyType::RIGHT);
                        default:
                            break;
                    }
            }
        }

        windows::XinputState state;
        windows::createZeroMemory(&state, sizeof(windows::XinputState));
        const auto dwResult = windows::XinputGetState(&state);
        if (dwResult == windows::XINPUT_SUCCESS) {
            if (state.Gamepad.wButtons & windows::XINPUT_A) {
                return this->notifyAndThrottle(KeyType::CONFIRM);
            } else if (state.Gamepad.wButtons & windows::XINPUT_B) {
                return this->notifyAndThrottle(KeyType::CANCEL);
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_UP) {
                return this->notifyAndThrottle(KeyType::UP);
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_DOWN) {
                return this->notifyAndThrottle(KeyType::DOWN);
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_RIGHT) {
                return this->notifyAndThrottle(KeyType::RIGHT);
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_LEFT) {
                return this->notifyAndThrottle(KeyType::LEFT);
            }
        }
    }

    void notifyAndFlush(const KeyType event) {
        easyx::flushMessage(easyx::MESSAGE_TYPE_MOUSE | easyx::MESSAGE_TYPE_KEY);
        this->notify(event);
    }

    void notifyAndThrottle(const KeyType event) {
        if (throttleFor(150, "keypress")) {
            this->notify(event);
        }
    }

    static SubjectKeyPress *getInstance() {
        mutex_.lock();
        if (instance_ == nullptr) {
            instance_ = new SubjectKeyPress();
        }
        mutex_.unlock();

        return instance_;
    }
};

export class ObservesKeyPress : public IObserver<KeyType> {
public:
    ObservesKeyPress() {
        SubjectKeyPress::getInstance()->attach(this);
    }

    ~ObservesKeyPress() override {
        SubjectKeyPress::getInstance()->detach(this);
    }

    void update(const KeyType &message) final {
        this->onKeyPress(message);
    }

    virtual void onKeyPress(KeyType keyType) = 0;
};

export class SubjectKeyHold : public ISubject<bool> {
private:
    static inline SubjectKeyHold *instance_{nullptr};
    static inline std::mutex mutex_;

public:
    void update() {
        /**
         * Only observes controller X button, SPACE key, and mouse LEFT button
         * For snake acceleration
         */
        windows::XinputState state;
        windows::createZeroMemory(&state, sizeof(windows::XinputState));
        const auto dwResult = windows::XinputGetState(&state);
        if (dwResult == windows::XINPUT_SUCCESS) {
            if (state.Gamepad.wButtons & windows::XINPUT_X) {
                return this->notify(true);
            }
        }

        if (windows::isKeyDown(windows::VKEY_SPACE) ||
            windows::isKeyDown(windows::VKEY_MOUSE_L)) {
            return this->notify(true);
        }

        this->notify(false);
    }

    static SubjectKeyHold *getInstance() {
        mutex_.lock();
        if (instance_ == nullptr) {
            instance_ = new SubjectKeyHold();
        }
        mutex_.unlock();

        return instance_;
    }
};

export class ObservesKeyHold : public IObserver<bool> {
public:
    ObservesKeyHold() {
        SubjectKeyHold::getInstance()->attach(this);
    }

    ~ObservesKeyHold() override {
        SubjectKeyHold::getInstance()->detach(this);
    }

    void update(const bool &message) final {
        this->onKeyHold(message);
    }

    virtual void onKeyHold(bool hold) = 0;
};
