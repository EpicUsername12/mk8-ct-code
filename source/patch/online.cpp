#include "online.hpp"

ctgp::OnlinePatch* ctgp::OnlinePatchInstance;
ui::Control_WifiCmnButton* onlineCtgpToggleBtn = NULL;

net::NetworkData_Menu* currentNetworkData = NULL;
DECL_CXX_HOOK(int, net::NetworkData_Menu::init, net::NetworkData_Menu* _this) {
    currentNetworkData = _this;
    return real(_this);
}

struct LobbyPlayer {
    int field_0;
    enl::UniqueID playerId;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
};

DECL_CXX_HOOK(int, net::NetworkData_Menu::getRouletteResult, void* _this) {

    int ret = real(_this);
    if (ctgp::GlobalPatchInstance->getOnlineToggle() &&
        nn::pia::transport::StationManager::getInstance()->localStation->stationIndex == currentNetworkData->m_CourseChoice.m_PeerInfo.m_StationId) {
        for (int i = 0; i < 12; i++) {
            if (currentNetworkData->m_Players[i].m_PeerInfo.m_StationId != 253) {
                uint32_t* buffer = (uint32_t*)calloc(1, 0x554);
                buffer[0] = 0x43544750;
                buffer[1] = ctgp::CoursePatchInstance->m_CurrentPageNum;
                ctgp::OnlinePatch::SendToStation(currentNetworkData->m_Players[i].m_PeerInfo.m_StationId, buffer);
                free(buffer);
            }
        }
    }

    return ret;
}

void loadPageTrackPaths() {

    ui::Heap_Common* heap = ui::Heap_Common::getHeap();
    if (ctgp::CoursePatchInstance->m_CurrentPageNum > 0) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 4; j++) {
                ctgp::CUP0Instance->trackNamesMod[heap->courseData->courseList.cups[i].courses[j].id] =
                    ctgp::CUP0Instance->pages[ctgp::CoursePatchInstance->m_CurrentPageNum - 1]->trackPaths[i][j];
            }
        }
    } else if (ctgp::CoursePatchInstance->m_CurrentPageNum == 0) {
        for (int i = 0; i < 8; i++) {
            for (int i = 0; i < 4; i++) {
                ctgp::CUP0Instance->trackNamesMod[heap->courseData->courseList.cups[i].courses[i].id] = NULL;
            }
        }
    }
}

DECL_CXX_HOOK(int, nn::pia::inet::SocketInputStream::readFromSocket, nn::pia::inet::SocketInputStream* _this, uint32_t* data) {
    int ret = real(_this, data);
    if (!ret && ctgp::GlobalPatchInstance->getOnlineToggle() && data[0] == 0x43544750) {
        DBG_LOG("Received custom CTGP packet with page %d\n", data[1]);
        ctgp::CoursePatchInstance->m_CurrentPageNum = (int)data[1];
        loadPageTrackPaths();
    }
    return ret;
}

/* CTGP Region:

- 101: Default beta

*/

DECL_CXX_HOOK(int, nn::nex::MatchmakeSessionSearchCriteria::SetAttribute, nn::nex::MatchmakeSessionSearchCriteria* _this, uint32_t attributeIndex, uint32_t attributeValue) {
    if (attributeIndex == 3 && ctgp::GlobalPatchInstance->getOnlineToggle()) {
        attributeValue = 101;
    }
    if (attributeIndex == 3) {
        ctgp::GlobalPatchInstance->m_LastRegion = attributeValue;
    }
    int ret = real(_this, attributeIndex, attributeValue);
    return ret;
}

DECL_CXX_HOOK(int, nn::nex::MatchmakeSession::SetAttribute, nn::nex::MatchmakeSession* _this, uint32_t attributeIndex, uint32_t attributeValue) {
    if (attributeIndex == 3 && ctgp::GlobalPatchInstance->getOnlineToggle()) {
        attributeValue = 101;
    }
    if (attributeIndex == 3) {
        ctgp::GlobalPatchInstance->m_LastRegion = attributeValue;
    }
    int ret = real(_this, attributeIndex, attributeValue);
    return ret;
}

DECL_CXX_HOOK(int, ui::Page_WifiTop::onCreate, ui::Page_WifiTop* _this) {
    int ret = real(_this);
    if (!onlineCtgpToggleBtn)
        onlineCtgpToggleBtn = new ui::Control_WifiCmnButton(_this);
    onlineCtgpToggleBtn->field_00 = 4;
    onlineCtgpToggleBtn->flow = &_this->flow0;
    onlineCtgpToggleBtn->field_24 = 3;
    onlineCtgpToggleBtn->inputKey.requiredInput = VPAD_BUTTON_PLUS;
    _this->createControl_(onlineCtgpToggleBtn, _this->controls.mPtrs[0], "L_CTGP_00");
    onlineCtgpToggleBtn->setMessage(ONLINE_TOGGLE_MSG_ID);
    return ret;
}

//! TODO: Set matchmaking type to the faster matchmaking, we don't care about
//! score difference since low player base

bool Page_WifiTop_ButtonHandler(ui::Control_Button* _this, ui::UIEvent* event) {

    if (onlineCtgpToggleBtn && _this == onlineCtgpToggleBtn) { // 1 = A press, 0 = Touch pad
        if (event->m_InputValue <= 1 || event->m_InputValue == VPAD_BUTTON_PLUS) {
            ctgp::GlobalPatchInstance->setOnlineToggle(!ctgp::GlobalPatchInstance->getOnlineToggle());
            char16_t* ctgpText = (char16_t*)u"  Online CTGP: OFF";
            if (ctgp::GlobalPatchInstance->getOnlineToggle()) {
                ctgpText = (char16_t*)u"  Online CTGP: ON";
            }
            ctgpText[0] = 0xE045; // (+) sign unicode value for Nintendo font
            ModifyMessage(ctgpText, ONLINE_TOGGLE_MSG_ID, false);
            onlineCtgpToggleBtn->setMessage(ONLINE_TOGGLE_MSG_ID);
            audio::utl::startSound2D("SE_SYS_DIALOG_OPEN");

            if (!ctgp::GlobalPatchInstance->getOnlineToggle()) {
                ctgp::CoursePatchInstance->m_CurrentPageNum = 0;
                ctgp::CoursePatchInstance->LoadPage();
            }

            return true;
        }
    }

    return false;
}

DECL_CXX_HOOK(int, enl::PiaInetMatchingManager::autoMatch, enl::PiaInetMatchingManager* _this) {
    int ret = real(_this);
    /*
            if(ctgp::GlobalPatchInstance->m_IsRgsConnected) {
                    rgs::GatheringPlayer player;
                    player.m_JoinStatus = rgs::JOIN_STATUS_AUTOMATCH;
                    player.m_Region = ctgp::GlobalPatchInstance->m_LastRegion;
                    player.m_PrincipalID = _this->m_UserPid;
                    player.m_IsHost = _this->m_UserPid == _this->m_HostPid;
                    player.m_GatheringID = _this->m_GatheringId;
                    CustomTrackGrandPrixCafeServiceClient::UpdatePlayerDataAsync(&player,
       rgs::DefaultAsyncCallback);
            }
    */
    return ret;
}

DECL_CXX_HOOK(int, enl::PiaInetMatchingManager::joinMatch, enl::PiaInetMatchingManager* _this) {
    int ret = real(_this);
    /*
            if(ctgp::GlobalPatchInstance->m_IsRgsConnected) {
                    rgs::GatheringPlayer player;
                    player.m_JoinStatus = rgs::JOIN_STATUS_JOINMATCH;
                    player.m_Region = ctgp::GlobalPatchInstance->m_LastRegion;
                    player.m_PrincipalID = _this->m_UserPid;
                    player.m_IsHost = _this->m_UserPid == _this->m_HostPid;
                    player.m_GatheringID = _this->m_GatheringId;
                    CustomTrackGrandPrixCafeServiceClient::UpdatePlayerDataAsync(&player,
       rgs::DefaultAsyncCallback);
            }
    */
    return ret;
}

DECL_CXX_HOOK(int, enl::PiaInetMatchingManager::createMatch, enl::PiaInetMatchingManager* _this) {
    int ret = real(_this);
    /*
            if(ctgp::GlobalPatchInstance->m_IsRgsConnected) {
                    rgs::GatheringPlayer player;
                    player.m_JoinStatus = rgs::JOIN_STATUS_CREATEMATCH;
                    player.m_Region = ctgp::GlobalPatchInstance->m_LastRegion;
                    player.m_PrincipalID = _this->m_UserPid;
                    player.m_IsHost = _this->m_UserPid == _this->m_HostPid;
                    player.m_GatheringID = _this->m_GatheringId;
                    CustomTrackGrandPrixCafeServiceClient::UpdatePlayerDataAsync(&player,
       rgs::DefaultAsyncCallback);
            }
    */
    return ret;
}

DECL_CXX_HOOK(int, enl::PiaInetMatchingManager::disconnect, enl::PiaInetMatchingManager* _this) {
    int ret = real(_this);
    /*
            if(ctgp::GlobalPatchInstance->m_IsRgsConnected) {
                    rgs::GatheringPlayer player;
                    player.m_JoinStatus = rgs::JOIN_STATUS_DISCONNECT;
                    player.m_PrincipalID = _this->m_UserPid;
                    CustomTrackGrandPrixCafeServiceClient::UpdatePlayerDataAsync(&player,
       rgs::DefaultAsyncCallback);
            }
    */
    return ret;
}

DECL_CXX_HOOK(int, ui::UICourseList::getRandomOnlineTracks, ui::UICourseList* _this, ui::CourseInfo* trackOutputs, void* blacklist) {
    memset(ctgp::OnlinePatchInstance->m_TrackOptionsPage, 0, sizeof(ctgp::OnlinePatchInstance->m_TrackOptionsPage));
    int ret = real(_this, trackOutputs, blacklist);
    return ret;
}

void ctgp::OnlinePatch::Initialize() {

    ctgp::GlobalPatchInstance->RegisterButtonHandlerRedirection(ui::Page_WifiTop::ID, Page_WifiTop_ButtonHandler);

    MK8_HOOK_CXX_FUNC(0x023D3BA4, net::NetworkData_Menu::init);
    MK8_HOOK_CXX_FUNC(0x02411FF0, net::NetworkData_Menu::getRouletteResult);
    MK8_HOOK_CXX_FUNC(0x02A02944, nn::pia::inet::SocketInputStream::readFromSocket);
    MK8_HOOK_CXX_FUNC(0x0298E52C, nn::nex::MatchmakeSession::SetAttribute);
    MK8_HOOK_CXX_FUNC(0x0298E7B4, nn::nex::MatchmakeSessionSearchCriteria::SetAttribute);
    MK8_HOOK_CXX_FUNC(0x025F3A48, ui::Page_WifiTop::onCreate);
    MK8_HOOK_CXX_FUNC(0x028CBF70, enl::PiaInetMatchingManager::autoMatch);
    MK8_HOOK_CXX_FUNC(0x028CC520, enl::PiaInetMatchingManager::joinMatch);
    MK8_HOOK_CXX_FUNC(0x028CCC54, enl::PiaInetMatchingManager::createMatch)
    MK8_HOOK_CXX_FUNC(0x028CC678, enl::PiaInetMatchingManager::disconnect);
    MK8_HOOK_CXX_FUNC(0x0254404C, ui::UICourseList::getRandomOnlineTracks);
}