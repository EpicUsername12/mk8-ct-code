#ifndef _PATCH_FS_H
#define _PATCH_FS_H

#include <mutex>

#include "patch/course.hpp"
#include "private/crypto_engine.hpp"
#include "wiiu/debug.hpp"
#include "wiiu/patcher.h"

typedef struct fileHandleHook {
  bool active;
  int handle;
  int offset;
} fileHandleHook;

extern uint8_t private_ctgp_key[48];
extern FSClient *fsClient;
extern FSCmdBlock *fsCmdBlock;

void InitializePatchFilesystem();

uint8_t *GetArchiveFile(const char *filename, uint32_t *outSize = NULL,
                        uint32_t align = 0x40);

#endif