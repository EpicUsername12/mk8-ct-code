#ifndef _WIIU_DEBUGGER_H
#define _WIIU_DEBUGGER_H

#include "symbols.h"

void ExceptionHandler(const char *type, OSContext *context);

namespace ctgp::common {

/**
 * @brief Class holder for the UDP debugger used to send message to a computer
 * running the "udplogserver"
 *
 */
class UDPDebugger : public ctgp::common::RootClass {
private:
  ///! Socket mutex for concurrent log message
  OSMutex m_SocketMutex;

public:
  sockaddr_in m_SocketAddr;
  int m_SocketFd = -1;
  int m_Port = 4405;

public:
  /**
   * @brief Construct a new UDPDebugger object
   *
   * @param port UDP port (must be in range 0 > x < 65536)
   */
  UDPDebugger(int port = 4405);

  /**
   * @brief Sends a buffer to the datagram socket
   *
   * @param txt
   */
  void PrintLog(const char *txt);

  /**
   * @brief Format and send a buffer to the datagram socket
   *
   * @param fmt
   * @param ...
   */
  void PrintLogf(const char *fmt, ...);

  /**
   * @brief Open an UDP socket and feeds the socket address with the required
   * values
   *
   */
  virtual void Initialize();

  /**
   * @brief Get the Port object
   *
   * @return int UDP port (must be in range 0 > x < 65536)
   */
  int getPort() { return this->m_Port; }

  /**
   * @brief Set the Port object
   *
   * @param port UDP port (must be in range 0 > x < 65536)
   */
  void setPort(int port) { this->m_Port = port; }

  /**
   * @brief Locks the mutex
   *
   */
  void enterCriticalSection() { OSLockMutex(&m_SocketMutex); }

  /**
   * @brief Unlocks the mutex
   *
   */
  void leaveCriticalSection() { OSUnlockMutex(&m_SocketMutex); }

  /**
   * @brief Create a Instance of the UDPDebugger and initializes it
   *
   * @return ctgp::common::UDPDebugger*
   */
  static inline ctgp::common::UDPDebugger *createInstance() {
    ctgp::common::UDPDebugger *inst = new ctgp::common::UDPDebugger();
    inst->Initialize();
    return inst;
  }
};

///! UDPDebugger instance
extern UDPDebugger *UDPDebuggerInstance;
} // namespace ctgp::common

#define __FILENAME_X__                                                         \
  (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define __FILENAME__                                                           \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILENAME_X__)

#ifdef __DEBUG__
#define DBG_LOG(FMT, ARGS...)                                                  \
  do {                                                                         \
    printf("%s:%s:%d | " FMT, __FILENAME__, __FUNCTION__, __LINE__, ##ARGS);   \
  } while (0)
#else
#define DBG_LOG(FMT, ARGS...)
#endif

#endif
