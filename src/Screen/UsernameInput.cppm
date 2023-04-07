export module Screen.UsernameInput;

import <string>;

import Internal.GameStore;
import ThirdParty;
import Utils.TextEncode;

export class UsernameInput {
private:
    GameStore *store;

public:
    UsernameInput()
        : store{GameStore::getInstance()} {}

    void operator()() {
        std::string username{32, '\0'};

        bool hasInput = easyx::inputBox(username, encode("输入昵称..."));

        const char *realStr = username.data();
        if (hasInput && realStr[0]) {
            this->store->setUsername(realStr);
        }
    }
};
