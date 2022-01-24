#ifndef _PATCH_MSG_H
#define _PATCH_MSG_H

#include <mk8/sead/StringUtil/StringUtil.hpp>
#include <mk8/ui/UILoader.hpp>

#include "utils.hpp"
#include "wiiu/debug.hpp"
#include "wiiu/patcher.h"

#define MAX_MESSAGE_COUNT 256
#define SPEEDOMETER_MSG_ID 23337
#define ONLINE_TOGGLE_MSG_ID 23338

typedef struct MessageData {
  bool active;
  bool alloc;
  int id;
  char16_t *message;
} MessageData;

void InitializePatchMessage();
void AddMessage(char16_t *text, int id, bool alloc = false);
void ModifyMessage(char16_t *text, int id, bool alloc = true);
char16_t *fromUTF8(char *text);

extern MessageData messages[MAX_MESSAGE_COUNT];

#endif