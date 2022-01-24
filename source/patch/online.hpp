#ifndef _PATCH_ONLINE_H
#define _PATCH_ONLINE_H

#include <mk8/audio/utl/AudioUtil.hpp>
#include <mk8/enl/MatchingManager.hpp>
#include <mk8/net/NetworkTransporter_Menu.h>
#include <mk8/nn/nex/MatchMaking.hpp>
#include <mk8/nn/pia.h>
#include <mk8/nw/lyt/Resource.hpp>
#include <mk8/sead/StringUtil/StringUtil.hpp>
#include <mk8/sys/SystemEngine.hpp>
#include <mk8/ui/Control_WifiCmnButton.hpp>
#include <mk8/ui/Heaps/Heap_Common.hpp>
#include <mk8/ui/Pages/Page_CourseGP.hpp>
#include <mk8/ui/Pages/Page_TitleTop.hpp>
#include <mk8/ui/Pages/Page_WifiTop.hpp>
#include <mk8/ui/UICourse.hpp>
#include <mk8/ui/UICursor.hpp>
#include <mk8/ui/UIEngine.hpp>
#include <mk8/ui/UIEvent.hpp>

#include "patch/course.hpp"
#include "patch/global_patch.hpp"
#include "patch/message.hpp"
#include "utils.hpp"
#include "wiiu/patcher.h"

#include <map>

namespace ctgp {

enum PacketID : uint32_t {
  PACKET_ID_SEND_TRACK_PAGE,
  PACKET_ID_SEND_TRACK_IDS,
};

class OnlinePatch : ctgp::common::RootClass {
private:
public:
  int m_TrackOptionsPage[4] = {0, 0, 0, 0};

public:
  /**
   * @brief Construct a new Online Patch object
   *
   */
  OnlinePatch() { this->setName("ctgp::OnlinePatch"); }

  /**
   * @brief Initialize the OnlinePatch object
   *
   */
  virtual void Initialize();

  static void SendToStation(int stationId, void *buffer) {
    sockaddr_in tmp_addr = {0};
    nn::pia::common::StationAddress client_addr;
    nn::pia::transport::StationManager::getInstance()->GetStationAddress(
        &client_addr, stationId);
    tmp_addr.sin_addr.s_addr = client_addr.inetAddress.host;
    tmp_addr.sin_port = client_addr.inetAddress.port;
    tmp_addr.sin_family = AF_INET;
    for (int i = 0; i < 4; i++)
      sendto(nn::pia::transport::ThreadStreamManager::getInstance()
                 ->getOutputStream()
                 ->socket->fd,
             buffer, 0x554, 0x60, (const sockaddr_in *)&tmp_addr, 16);
  }

  /**
   * @brief Create a Instance of OnlinePatch
   *
   * @return ctgp::OnlinePatch*
   */
  static inline ctgp::OnlinePatch *createInstance() {
    ctgp::OnlinePatch *inst = new ctgp::OnlinePatch();
    inst->Initialize();
    return inst;
  }
};

extern OnlinePatch *OnlinePatchInstance;

} // namespace ctgp

#endif