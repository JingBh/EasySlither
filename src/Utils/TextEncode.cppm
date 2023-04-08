export module Utils.TextEncode;

import <string>;

import ThirdParty;

export std::string encode(std::string text) {
    switch (windows::getActiveCodePage()) {
        case 936: // GBK
            return Utf8ToGbk(text.data());
        default:
            return text;
    }
}

export std::string decode(std::string text) {
    switch (windows::getActiveCodePage()) {
        case 936: // GBK
            return GbkToUtf8(text.data());
        default:
            return text;
    }
}
