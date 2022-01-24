#include "utils.hpp"

void KernelCopyData(unsigned int addr, unsigned int src, unsigned int len) {

  /*
   * Setup a DBAT access with cache inhibited to write through and read directly
   * from memory
   */

  // save the original DBAT value
  unsigned int dbatu0, dbatl0, dbatu1, dbatl1;
  asm volatile("mfdbatu %0, 0" : "=r"(dbatu0));
  asm volatile("mfdbatl %0, 0" : "=r"(dbatl0));
  asm volatile("mfdbatu %0, 1" : "=r"(dbatu1));
  asm volatile("mfdbatl %0, 1" : "=r"(dbatl1));

  unsigned int target_dbatu0 = 0;
  unsigned int target_dbatl0 = 0;
  unsigned int target_dbatu1 = 0;
  unsigned int target_dbatl1 = 0;

  unsigned char *dst_p = (unsigned char *)addr;
  unsigned char *src_p = (unsigned char *)src;

  // we only need DBAT modification for addresses out of our own DBAT range
  // as our own DBAT is available everywhere for user and supervisor
  // since our own DBAT is on DBAT5 position we don't collide here
  if (addr < 0x00800000 || addr >= 0x01000000) {
    target_dbatu0 = (addr & 0x00F00000) | 0xC0000000 | 0x1F;
    target_dbatl0 = (addr & 0xFFF00000) | 0x32;
    asm volatile("mtdbatu 0, %0" : : "r"(target_dbatu0));
    asm volatile("mtdbatl 0, %0" : : "r"(target_dbatl0));
    dst_p = (unsigned char *)((addr & 0xFFFFFF) | 0xC0000000);
  }
  if (src < 0x00800000 || src >= 0x01000000) {
    target_dbatu1 = (src & 0x00F00000) | 0xB0000000 | 0x1F;
    target_dbatl1 = (src & 0xFFF00000) | 0x32;

    asm volatile("mtdbatu 1, %0" : : "r"(target_dbatu1));
    asm volatile("mtdbatl 1, %0" : : "r"(target_dbatl1));
    src_p = (unsigned char *)((src & 0xFFFFFF) | 0xB0000000);
  }

  asm volatile("eieio; isync");

  unsigned int i;
  for (i = 0; i < len; i++) {
    // if we are on the edge to next chunk
    if ((target_dbatu0 != 0) &&
        (((unsigned int)dst_p & 0x00F00000) != (target_dbatu0 & 0x00F00000))) {
      target_dbatu0 = ((addr + i) & 0x00F00000) | 0xC0000000 | 0x1F;
      target_dbatl0 = ((addr + i) & 0xFFF00000) | 0x32;
      dst_p = (unsigned char *)(((addr + i) & 0xFFFFFF) | 0xC0000000);

      asm volatile("eieio; isync");
      asm volatile("mtdbatu 0, %0" : : "r"(target_dbatu0));
      asm volatile("mtdbatl 0, %0" : : "r"(target_dbatl0));
      asm volatile("eieio; isync");
    }
    if ((target_dbatu1 != 0) &&
        (((unsigned int)src_p & 0x00F00000) != (target_dbatu1 & 0x00F00000))) {
      target_dbatu1 = ((src + i) & 0x00F00000) | 0xB0000000 | 0x1F;
      target_dbatl1 = ((src + i) & 0xFFF00000) | 0x32;
      src_p = (unsigned char *)(((src + i) & 0xFFFFFF) | 0xB0000000);

      asm volatile("eieio; isync");
      asm volatile("mtdbatu 1, %0" : : "r"(target_dbatu1));
      asm volatile("mtdbatl 1, %0" : : "r"(target_dbatl1));
      asm volatile("eieio; isync");
    }

    *dst_p = *src_p;

    ++dst_p;
    ++src_p;
  }

  /*
   * Restore original DBAT value
   */
  asm volatile("eieio; isync");
  asm volatile("mtdbatu 0, %0" : : "r"(dbatu0));
  asm volatile("mtdbatl 0, %0" : : "r"(dbatl0));
  asm volatile("mtdbatu 1, %0" : : "r"(dbatu1));
  asm volatile("mtdbatl 1, %0" : : "r"(dbatl1));
  asm volatile("eieio; isync");
}