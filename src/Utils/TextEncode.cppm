export module Utils.TextEncode;

import <string>;

import ThirdParty;

export std::string encode(std::string text) {
    switch (windows::getActiveCodePage()) {
        case 936:
            return Utf8ToGbk(text.data());
        default:
            return text;
    }
}
