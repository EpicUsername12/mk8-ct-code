#ifndef _AA_PPC_BAT_H
#define _AA_PPC_BAT_H

#include "ppc.hpp"

constexpr inline const char *BlockAreaToString(uint32_t BL) {
  switch (BL) {
  case BAT_AREA_128K:
    return "128K";
  case BAT_AREA_256K:
    return "256K";
  case BAT_AREA_512K:
    return "512K";
  case BAT_AREA_1M:
    return "1M";
  case BAT_AREA_2M:
    return "2M";
  case BAT_AREA_4M:
    return "4M";
  case BAT_AREA_8M:
    return "8M";
  case BAT_AREA_16M:
    return "16M";
  case BAT_AREA_32M:
    return "32M";
  case BAT_AREA_64M:
    return "64M";
  case BAT_AREA_128M:
    return "128M";
  case BAT_AREA_256M:
    return "256M";
  default:
    return "<?>";
  }
  return "<?>";
}

constexpr inline uint32_t GetBATU(uint32_t vaddr, uint32_t BL, bool supervisor,
                                  bool user) {
  return ((vaddr & BATU_BEPI_MASK) | ((BL & BATU_BL_MASK) << BATU_BL_SHIFT) |
          ((supervisor & 1) << 1) | (user & 1));
}

constexpr inline uint32_t GetBATL(uint32_t paddr, uint32_t WIMG, bool read,
                                  bool write) {
  uint32_t pp = 0;
  if (read && write)
    pp = 2;
  else if (read && !write)
    pp = 1;
  else
    pp = 0;

  return (paddr & BATL_BRPN_MASK) |
         ((WIMG & BATL_WIMG_MASK) << BATL_WIMG_SHIFT) | pp;
}

#endif