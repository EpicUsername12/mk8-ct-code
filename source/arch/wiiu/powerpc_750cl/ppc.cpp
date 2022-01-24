#include "ppc.hpp"

void __ReadKernelPhysical(uint32_t pa_dest, uint32_t src, size_t size) {
    uint32_t backupMSR;
    asm volatile("mfmsr %0" : "=r"(backupMSR));
    uint32_t pMSR = backupMSR & ~MSR_DR_BIT;
    for (uint32_t i = 0; i < size; i += 4) {
        asm volatile("mtmsr %0" : : "r"(pMSR));
        uint32_t val = *(uint32_t*)(src + i);
        asm volatile("mtmsr %0" : : "r"(backupMSR));
        *(uint32_t*)(pa_dest + i) = val;
    }
}

void __WriteKernelPhysical(uint32_t pa_dest, uint32_t src, uint32_t size) {
    uint32_t backupMSR;
    asm volatile("mfmsr %0" : "=r"(backupMSR));
    uint32_t pMSR = backupMSR & ~MSR_DR_BIT;
    for (uint32_t i = 0; i < size; i += 4) {
        asm volatile("mtmsr %0" : : "r"(backupMSR));
        uint32_t val = *(uint32_t*)(src + i);
        asm volatile("mtmsr %0" : : "r"(pMSR));
        *(uint32_t*)(pa_dest + i) = val;
    }
    asm volatile("mtmsr %0" : : "r"(backupMSR));
}

void __GetMemoryRegisters(PowerPC_MemoryRegisters* data) {
    __asm__ __volatile__("eieio; isync; sync;");

    // SPR 25 -> SDR1
    __asm__ __volatile__("mfspr %0, 25;" : "=r"(data->sdr1));

    // Copy all segment registers into the table
    __asm__ __volatile__("mfsr %0, 0 ;" : "=r"(data->sr[0x00]));
    __asm__ __volatile__("mfsr %0, 1 ;" : "=r"(data->sr[0x01]));
    __asm__ __volatile__("mfsr %0, 2 ;" : "=r"(data->sr[0x02]));
    __asm__ __volatile__("mfsr %0, 3 ;" : "=r"(data->sr[0x03]));
    __asm__ __volatile__("mfsr %0, 4 ;" : "=r"(data->sr[0x04]));
    __asm__ __volatile__("mfsr %0, 5 ;" : "=r"(data->sr[0x05]));
    __asm__ __volatile__("mfsr %0, 6 ;" : "=r"(data->sr[0x06]));
    __asm__ __volatile__("mfsr %0, 7 ;" : "=r"(data->sr[0x07]));
    __asm__ __volatile__("mfsr %0, 8 ;" : "=r"(data->sr[0x08]));
    __asm__ __volatile__("mfsr %0, 9 ;" : "=r"(data->sr[0x09]));
    __asm__ __volatile__("mfsr %0, 10;" : "=r"(data->sr[0x0A]));
    __asm__ __volatile__("mfsr %0, 11;" : "=r"(data->sr[0x0B]));
    __asm__ __volatile__("mfsr %0, 12;" : "=r"(data->sr[0x0C]));
    __asm__ __volatile__("mfsr %0, 13;" : "=r"(data->sr[0x0D]));
    __asm__ __volatile__("mfsr %0, 14;" : "=r"(data->sr[0x0E]));
    __asm__ __volatile__("mfsr %0, 15;" : "=r"(data->sr[0x0F]));

    // DBAT0U -> DBAT7U
    __asm__ __volatile__("mfspr %0, 536;" : "=r"(data->dbatu[0]));
    __asm__ __volatile__("mfspr %0, 538;" : "=r"(data->dbatu[1]));
    __asm__ __volatile__("mfspr %0, 540;" : "=r"(data->dbatu[2]));
    __asm__ __volatile__("mfspr %0, 542;" : "=r"(data->dbatu[3]));
    __asm__ __volatile__("mfspr %0, 568;" : "=r"(data->dbatu[4]));
    __asm__ __volatile__("mfspr %0, 570;" : "=r"(data->dbatu[5]));
    __asm__ __volatile__("mfspr %0, 572;" : "=r"(data->dbatu[6]));
    __asm__ __volatile__("mfspr %0, 574;" : "=r"(data->dbatu[7]));

    // DBAT0L -> DBAT7L
    __asm__ __volatile__("mfspr %0, 537;" : "=r"(data->dbatl[0]));
    __asm__ __volatile__("mfspr %0, 539;" : "=r"(data->dbatl[1]));
    __asm__ __volatile__("mfspr %0, 541;" : "=r"(data->dbatl[2]));
    __asm__ __volatile__("mfspr %0, 543;" : "=r"(data->dbatl[3]));
    __asm__ __volatile__("mfspr %0, 569;" : "=r"(data->dbatl[4]));
    __asm__ __volatile__("mfspr %0, 571;" : "=r"(data->dbatl[5]));
    __asm__ __volatile__("mfspr %0, 573;" : "=r"(data->dbatl[6]));
    __asm__ __volatile__("mfspr %0, 575;" : "=r"(data->dbatl[7]));

    // IBAT0U -> IBAT7U
    __asm__ __volatile__("mfspr %0, 528;" : "=r"(data->ibatu[0]));
    __asm__ __volatile__("mfspr %0, 530;" : "=r"(data->ibatu[1]));
    __asm__ __volatile__("mfspr %0, 532;" : "=r"(data->ibatu[2]));
    __asm__ __volatile__("mfspr %0, 534;" : "=r"(data->ibatu[3]));
    __asm__ __volatile__("mfspr %0, 560;" : "=r"(data->ibatu[4]));
    __asm__ __volatile__("mfspr %0, 562;" : "=r"(data->ibatu[5]));
    __asm__ __volatile__("mfspr %0, 564;" : "=r"(data->ibatu[6]));
    __asm__ __volatile__("mfspr %0, 566;" : "=r"(data->ibatu[7]));

    // IBAT0L -> IBAT7L
    __asm__ __volatile__("mfspr %0, 529;" : "=r"(data->ibatl[0]));
    __asm__ __volatile__("mfspr %0, 531;" : "=r"(data->ibatl[1]));
    __asm__ __volatile__("mfspr %0, 533;" : "=r"(data->ibatl[2]));
    __asm__ __volatile__("mfspr %0, 535;" : "=r"(data->ibatl[3]));
    __asm__ __volatile__("mfspr %0, 561;" : "=r"(data->ibatl[4]));
    __asm__ __volatile__("mfspr %0, 563;" : "=r"(data->ibatl[5]));
    __asm__ __volatile__("mfspr %0, 565;" : "=r"(data->ibatl[6]));
    __asm__ __volatile__("mfspr %0, 567;" : "=r"(data->ibatl[7]));

    __asm__ __volatile__("eieio; isync; sync;");
}