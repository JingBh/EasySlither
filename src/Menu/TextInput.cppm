export module Menu.TextInput;

import Menu;

void TextInput::onTextInput(char character) {
    switch (character) {
        case '\b':
            if (!this->value->empty()) {
                this->value->pop_back();
            }
            break;
        case '\r':
        case '\n':
            break;
        default:
            if (this->value->size() < this->maxLength) {
                this->value->push_back(character);
            }
    }
}
