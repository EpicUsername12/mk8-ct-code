#ifndef _UTLS_H
#define _UTLS_H

#define MK8_J 0x000500001010EB00
#define MK8_U 0x000500001010EC00
#define MK8_E 0x000500001010ED00

#define MII_J 0x000500101004A000
#define MII_U 0x000500101004A100
#define MII_E 0x000500101004A200
#define HBL_A 0x0005000013374842

#define PROJECT_NAME "Mario Kart 8 CTGP"
#define PROJECT_VER_MAJOR 1
#define PROJECT_VER_VERSION 1
#define PROJECT_VER_MINOR 0
#define PROJECT_VER_DISC "beta"
#define PROJECT_AUTHOR "Rambo6Glaz"

#define PROJECT_PRIV_ALPHA 0
#define PROJECT_PRIV_BETA 1
#define PROJECT_BIN_VER                                                        \
  ((PROJECT_PRIV_ALPHA << 28) | (PROJECT_PRIV_BETA << 24) |                    \
   (PROJECT_VER_MAJOR << 16) | (PROJECT_VER_VERSION << 8) | PROJECT_VER_MINOR)

#include <cstring>
#include <gctypes.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void FatalError(const char *titleText, const char *errorText);

extern "C" uint32_t GetGPR13();
extern "C" void kern_write(void *addr, uint32_t value);
extern "C" void KernelCopyData(unsigned int addr, unsigned int src,
                               unsigned int len);
extern "C" void SC0x25_KernelCopyData(uint32_t addr, uint32_t src, size_t len);

#define C_UNLESS(expr, code)                                                   \
  ({                                                                           \
    if (!(expr)) {                                                             \
      code;                                                                    \
    }                                                                          \
  })

#define R_UNLESS(expr, res)                                                    \
  ({                                                                           \
    if (!(expr)) {                                                             \
      return res;                                                              \
    }                                                                          \
  })

#define FIND_ENTRY(arr, max_c, value)                                          \
  auto ret_idx = max_c;                                                        \
  for (auto i = 0; i < max_c; i++) {                                           \
    if (arr[i] == value) {                                                     \
      ret_idx = i;                                                             \
      break;                                                                   \
    }                                                                          \
  }

#define FIND_ENTRY_MEMBER(arr, max_c, member, value)                           \
  auto ret_idx = max_c;                                                        \
  for (auto i = 0; i < max_c; i++) {                                           \
    if (arr[i].##member == value) {                                            \
      ret_idx = i;                                                             \
      break;                                                                   \
    }                                                                          \
  }

#define FIND_ENTRY_MEMBER_(arr, max_c, member, value)                          \
  auto ret_idx = max_c;                                                        \
  for (auto i = 0; i < max_c; i++) {                                           \
    if (arr[i]->##member == value) {                                           \
      ret_idx = i;                                                             \
      break;                                                                   \
    }                                                                          \
  }

#define FIND_ENTRY_MEMBER_EX(arr, max_c, member, value, code)                  \
  for (auto i = 0; i < max_c; i++) {                                           \
    if (arr[i].member == value) {                                              \
      code;                                                                    \
      break;                                                                   \
    }                                                                          \
  }

#define FIND_ENTRY_MEMBER_EX_(arr, max_c, member, value, code)                 \
  for (auto i = 0; i < max_c; i++) {                                           \
    if (arr[i]->##member == value) {                                           \
      code;                                                                    \
      break;                                                                   \
    }                                                                          \
  }

#define RETURN_TO_HBL (0)
#define RETURN_TO_NEXT_APP (-3)

namespace ctgp::common {

class RootClass {
private:
  const char *m_Name = "<Undefined>";
  bool m_IsInitialized = false;

public:
  const char *getName() { return m_Name; }

  void setName(const char *name) { m_Name = name; }

  virtual void Initialize() { return; }
};

} // namespace ctgp::common

#endif