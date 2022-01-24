#include "debug.hpp"

ctgp::common::UDPDebugger* ctgp::common::UDPDebuggerInstance = NULL;

/* BEGINNING OF STATIC DEBUG HANDLER SECTION */

void ExceptionHandler(const char* type, OSContext* context) {

    char* symbolname = (char*)malloc(129);
    char* symbolname2 = (char*)malloc(129);

    bool valid = OSIsAddressValid(context->srr0);
    C_UNLESS(!valid, OSGetSymbolName(context->srr0, symbolname, 128));
    C_UNLESS(valid, memcpy(symbolname, "???", 19));

    valid = OSIsAddressValid(context->lr);
    C_UNLESS(!valid, OSGetSymbolName(context->lr, symbolname2, 128));
    C_UNLESS(valid, memcpy(symbolname2, "???", 19));

    char* lrBuffer = (char*)malloc(0x80);
    __os_snprintf(lrBuffer, 0x80, "0x%08x (%s)", context->lr, symbolname2);

    char* buffer = (char*)malloc(0x500);
    __os_snprintf(buffer, 2048,
                  "Exception: %s - in %s\n"
                  "Title ID: %016llX | Core: %d | UPID: %d\n\n"
                  ""
                  "An error has occured.\n"
                  "Press the POWER button for 4 seconds to shutdown.\n\n"
                  ""
                  "r%-2d: 0x%08X | r%-2d: 0x%08X | r%-2d: 0x%08X\n"
                  "r%-2d: 0x%08X | r%-2d: 0x%08X | r%-2d: 0x%08X\n"
                  "r%-2d: 0x%08X | r%-2d: 0x%08X | r%-2d: 0x%08X\n"
                  "r%-2d: 0x%08X | r%-2d: 0x%08X | r%-2d: 0x%08X\n"
                  "r%-2d: 0x%08X | r%-2d: 0x%08X | r%-2d: 0x%08X\n"
                  "r%-2d: 0x%08X | r%-2d: 0x%08X | r%-2d: 0x%08X\n"
                  "r%-2d: 0x%08X | r%-2d: 0x%08X | r%-2d: 0x%08X\n"
                  "r%-2d: 0x%08X | r%-2d: 0x%08X | r%-2d: 0x%08X\n"
                  "r%-2d: 0x%08X | r%-2d: 0x%08X | r%-2d: 0x%08X\n"
                  "r%-2d: 0x%08X | r%-2d: 0x%08X | r%-2d: 0x%08X\n"
                  "r%-2d: 0x%08X | r%-2d: 0x%08X | LR : %s\n"
                  "PC : 0x%08X | DAR: 0x%08X | DSISR: 0x%08X\n",
                  type, symbolname, OSGetTitleID(), OSGetCoreId(), OSGetUPID(), 0, context->gpr[0], 11, context->gpr[11], 22, context->gpr[22], 1, context->gpr[1], 12,
                  context->gpr[12], 23, context->gpr[23], 2, context->gpr[2], 13, context->gpr[13], 24, context->gpr[24], 3, context->gpr[3], 14, context->gpr[14], 25,
                  context->gpr[25], 4, context->gpr[4], 15, context->gpr[15], 26, context->gpr[26], 5, context->gpr[5], 16, context->gpr[16], 27, context->gpr[27], 6,
                  context->gpr[6], 17, context->gpr[17], 28, context->gpr[28], 7, context->gpr[7], 18, context->gpr[18], 29, context->gpr[29], 8, context->gpr[8], 19,
                  context->gpr[19], 30, context->gpr[30], 9, context->gpr[9], 20, context->gpr[20], 31, context->gpr[31], 10, context->gpr[10], 21, context->gpr[21], lrBuffer,
                  context->srr0, context->dar, context->dsisr);

    ctgp::common::UDPDebuggerInstance->PrintLog(buffer);
    socketclose(ctgp::common::UDPDebuggerInstance->m_SocketFd);
    OSFatal(buffer);
}

bool ExcDSI(OSContext* ctx) {
    ExceptionHandler("DSI", ctx);
    return false;
}

bool ExcISI(OSContext* ctx) {
    ExceptionHandler("ISI", ctx);
    return false;
}

bool ExcProgram(OSContext* ctx) {
    ExceptionHandler("Program", ctx);
    return false;
}

/* END OF STATIC DEBUG HANDLER SECTION */

ctgp::common::UDPDebugger::UDPDebugger(int port) {
    this->setPort(port);
    this->setName("ctgp::common::UDPDebugger");
    memset(&this->m_SocketAddr, 0, sizeof(sockaddr_in));
}

void ctgp::common::UDPDebugger::Initialize() {

    OSInitMutexEx(&m_SocketMutex, "UDP Debug Socket I/O Mutex");

    UDPDebuggerInstance = this;

    int broadcastEnable = 1;
    this->m_SocketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    setsockopt(this->m_SocketFd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

    this->m_SocketAddr.sin_family = AF_INET;
    this->m_SocketAddr.sin_port = this->getPort();
    this->m_SocketAddr.sin_addr.s_addr = INADDR_BROADCAST;

#ifdef __DEBUG__
    printf("\n\nStarted debug logger.\nProject: %s v%d.%d.%d-%s by %s\n\n", PROJECT_NAME, PROJECT_VER_MAJOR, PROJECT_VER_VERSION, PROJECT_VER_MINOR, PROJECT_VER_DISC,
           PROJECT_AUTHOR);
    OSSetExceptionCallbackEx(OS_EXCEPTION_MODE_GLOBAL_ALL_CORES, OS_EXCEPTION_TYPE_DSI, (void*)ExcDSI);
    OSSetExceptionCallbackEx(OS_EXCEPTION_MODE_GLOBAL_ALL_CORES, OS_EXCEPTION_TYPE_ISI, (void*)ExcISI);
    OSSetExceptionCallbackEx(OS_EXCEPTION_MODE_GLOBAL_ALL_CORES, OS_EXCEPTION_TYPE_PROGRAM, (void*)ExcProgram);
#endif
}

void ctgp::common::UDPDebugger::PrintLog(const char* txt) {

#ifdef __DEBUG__

    this->enterCriticalSection();

    // conveniently "stolen" from libutils lol
    int len = strlen(txt);
    int ret;
    while (len > 0) {
        int block = len < 1400 ? len : 1400;
        ret = sendto(this->m_SocketFd, txt, block, 0, &this->m_SocketAddr, sizeof(sockaddr_in));
        if (ret < 0)
            break;

        len -= ret;
        txt += ret;
    }

    this->leaveCriticalSection();

#endif
}

int (*__os_vsnprintf)(char* buffer, size_t size, const char* fmt, va_list) = (int (*)(char*, size_t, const char*, va_list))(0x2012D18 - 0xFE3C00);
void ctgp::common::UDPDebugger::PrintLogf(const char* fmt, ...) {

    va_list va;
    char* buf1 = (char*)malloc(0x800);

    va_start(va, fmt);
    __os_vsnprintf(buf1, 0x800, fmt, va);

    this->PrintLog(buf1);
    free(buf1);

    va_end(va);
}

extern "C" int __wrap_printf(const char* fmt, ...) {

    va_list va;
    char* buf1 = (char*)malloc(0x800);

    va_start(va, fmt);
    int ret = __os_vsnprintf(buf1, 0x800, fmt, va);

    ctgp::common::UDPDebuggerInstance->PrintLog(buf1);
    free(buf1);

    va_end(va);

    return ret;
}

extern "C" int __wrap_puts(const char* txt) {
    ctgp::common::UDPDebuggerInstance->PrintLog(txt);
    ctgp::common::UDPDebuggerInstance->PrintLog("\n");
    return strlen(txt);
}