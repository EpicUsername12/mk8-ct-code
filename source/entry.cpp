#include "main.hpp"

static int alreadyDoneIOSKernelExploit = 0;
extern "C" int __entry(int argc, char **argv) {

  LoadWiiUSymbols();
  ctgp::common::UDPDebugger::createInstance();
  ctgp::crypto::CryptoEngine::createInstance();

  uint64_t currentTitleId = OSGetTitleID();

  if (alreadyDoneIOSKernelExploit && currentTitleId != MK8_J &&
      currentTitleId != MK8_U && currentTitleId != MK8_E) {
    return RETURN_TO_NEXT_APP;
  }

  /* Patch kernel with our copy data syscall */
  kern_write((void *)(0xFFE84C70 + (0x25 * 4)), (uint32_t)KernelCopyData);
  kern_write((void *)(0xFFE85070 + (0x25 * 4)), (uint32_t)KernelCopyData);
  kern_write((void *)(0xFFE85470 + (0x25 * 4)), (uint32_t)KernelCopyData);
  kern_write((void *)(0xFFEAAA60 + (0x25 * 4)), (uint32_t)KernelCopyData);
  kern_write((void *)(0xFFEAAE60 + (0x25 * 4)), (uint32_t)KernelCopyData);

  if (!alreadyDoneIOSKernelExploit)
    IOSU_Exploit(&alreadyDoneIOSKernelExploit);

  OSThread *mainCtgpThread = (OSThread *)malloc(0x1000);
  uint32_t *stack = (uint32_t *)malloc(0x8000);
  bool found_one_title = false;

  if ((currentTitleId != MK8_J) && (currentTitleId != MK8_U) &&
      (currentTitleId != MK8_E)) {
    C_UNLESS(!SYSCheckTitleExists(MK8_J), SYSLaunchTitle(MK8_J);
             found_one_title = true);
    C_UNLESS(!SYSCheckTitleExists(MK8_U), SYSLaunchTitle(MK8_U);
             found_one_title = true);
    C_UNLESS(!SYSCheckTitleExists(MK8_E), SYSLaunchTitle(MK8_E);
             found_one_title = true);
    C_UNLESS(found_one_title, SYSLaunchMenu());
    return RETURN_TO_NEXT_APP;
  }

  OSCreateThread(mainCtgpThread, (void *)MainThread, argc, (char *)argv,
                 (void *)((uint32_t)stack + 0x8000), 0x2000, 0,
                 OS_THREAD_ATTRIB_AFFINITY_CPU1 | OS_THREAD_ATTRIB_DETACHED);
  OSResumeThread(mainCtgpThread);

  return RETURN_TO_NEXT_APP;
}
