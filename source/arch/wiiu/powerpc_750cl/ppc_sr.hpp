#ifndef _AA_PPC_SR_H
#define _AA_PPC_SR_H

#include "ppc.hpp"

constexpr inline uint32_t
CalculatePTEGAddress(uint32_t sdr1, uint32_t pageIndex, uint32_t hash) {
  uint32_t pageTable = sdr1 & SDR1_HTABORG_MASK;
  uint32_t pageMask = sdr1 & SDR1_HTABMASK_MASK;
  uint32_t maskedHash = hash & ((pageMask << 10) | 0x3FF);
  return pageTable | (maskedHash << 6);
}

constexpr inline void GetValidPTEGFromAddr(PowerPC_MemoryRegisters *regs,
                                           uint32_t *pageTable, int srId,
                                           uint32_t addr, uint32_t *PTEH,
                                           uint32_t *PTEL) {
  uint32_t sr = regs->sr[srId];
  uint32_t sdr1 = regs->sdr1;
  uint32_t VSID = sr & SR_VSID_MASK;

  if (sr & SR_T_BIT) {
    return;
  }

  /* Espresso feature only: Large page mode = 128 KB page instead of 4 KB */
  uint32_t pageIndex = (addr >> 17) & 0x7FF;
  uint32_t hash = (VSID & 0x7FFFF) ^ pageIndex;

  /* Calculate PTEG address from Effective Address */
  uint32_t primaryPTEG = CalculatePTEGAddress(sdr1, pageIndex, hash);
  uint32_t secondaryPTEG = CalculatePTEGAddress(sdr1, pageIndex, ~hash);

  // Loop through each primary PTEs
  for (int j = 0; j < 8; j++) {
    uint32_t idx = ((primaryPTEG + (j * 8)) - (sdr1 & SDR1_HTABORG_MASK)) / 4;
    uint32_t currentPTEH = pageTable[idx + 0];
    uint32_t currentPTEL = pageTable[idx + 1];

    /* V bit, 0 -> Invalid | 1 -> Valid */
    if (!(currentPTEH & PTEH_V_BIT))
      continue;

    /* First hash. */
    if (currentPTEH & PTEH_H_BIT)
      continue;

    /* SR VSID and entry VSID doesn't match.. */
    if (((currentPTEH >> PTEH_VSID_SHIFT) & PTEH_VSID_MASK) != VSID)
      continue;

    /* API != high bits of the page index */
    if ((currentPTEH & PTEH_API_MASK) != (pageIndex >> 5))
      continue;

    *PTEH = currentPTEH;
    *PTEL = currentPTEL;
    return;
  }

  /* Loop through each secondary PTEs */
  for (int j = 0; j < 8; j++) {
    uint32_t idx = ((secondaryPTEG + (j * 8)) - (sdr1 & SDR1_HTABORG_MASK)) / 4;
    uint32_t currentPTEH = pageTable[idx + 0];
    uint32_t currentPTEL = pageTable[idx + 1];

    /* V bit, 0 -> Invalid | 1 -> Valid */
    if (!(currentPTEH & PTEH_V_BIT))
      continue;

    /* Second hash. */
    if (!(currentPTEH & PTEH_H_BIT))
      continue;

    /* SR VSID and entry VSID doesn't match.. */
    if (((currentPTEH >> PTEH_VSID_SHIFT) & PTEH_VSID_MASK) != VSID)
      continue;

    /* API != high bits of the page index */
    if ((currentPTEH & PTEH_API_MASK) != (pageIndex >> 5))
      continue;

    *PTEH = currentPTEH;
    *PTEL = currentPTEL;
    return;
  }
}

#endif