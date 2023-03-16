export module Screen.UsernameInput;

import ThirdParty;
import Utils.TextEncode;

export void usernameInput() {
    char username[32];
    easyx::inputBox(username, 32, encode("输入昵称..."));
}
