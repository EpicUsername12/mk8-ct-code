#include "message.hpp"

/*
        The message patcher is global, and not complex, no need for a full C++
   rewrite
*/

MessageData messages[MAX_MESSAGE_COUNT];

DECL_CXX_HOOK(char16_t*, ui::UILoader::getMessage, ui::UILoader* _this, int messageId) {
    for (int i = 0; i < MAX_MESSAGE_COUNT; i++) {
        if (messages[i].active && messages[i].id == messageId) {
            return messages[i].message;
        }
    }

    return real(_this, messageId);
}

void InitializePatchMessage() {

    memset(messages, 0, sizeof(messages));
    MK8_HOOK_CXX_FUNC(MK8_UI_LOADER_GET_MESSAGE, ui::UILoader::getMessage);

    AddMessage((char16_t*)u"00 km/h", SPEEDOMETER_MSG_ID);

    char16_t* ctgpText = (char16_t*)u"  Online CTGP: OFF";
    ctgpText[0] = 0xE045;
    AddMessage(ctgpText, ONLINE_TOGGLE_MSG_ID);
}

void AddMessage(char16_t* text, int id, bool alloc) {

    for (int i = 0; i < MAX_MESSAGE_COUNT; i++) {
        if (!messages[i].active) {
            messages[i].active = true;
            messages[i].id = id;
            messages[i].message = text;
            messages[i].alloc = alloc;
            return;
        }
    }
}

void ModifyMessage(char16_t* text, int id, bool alloc) {
    for (int i = 0; i < MAX_MESSAGE_COUNT; i++) {
        if (messages[i].active && messages[i].id == id) {
            if (messages[i].alloc)
                free(messages[i].message);
            messages[i].message = text;
            messages[i].alloc = alloc;
            return;
        }
    }
}

char16_t* fromUTF8(char* text) {
    size_t l = strlen(text) + 1;
    char16_t* data = (char16_t*)calloc(l, 2);
    for (size_t i = 0; i < l; i++) {
        data[i] = text[i] & 0xff;
    }
    return data;
}