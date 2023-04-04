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

        easyx::inputBox(username, encode("输入昵称..."));

        if (!username.empty()) {
            store->username = username;
        }
    }
};
