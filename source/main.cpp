#include "main.hpp"

nn::erreula::AppearArg errorArg;
void FatalError(const char *titleText, const char *errorText) {
  if (errorArg.errorType != 1) {
    errorArg.errorType = 1;
    errorArg.errorCode = 4441337;
    errorArg.errorText = fromUTF8((char *)errorText);
    errorArg.titleText = fromUTF8((char *)titleText);
  }
}

void Page_TitleTop_AttachementRedirection(ui::UIPage *_this) {
  ((ui::Page_TitleTop *)(_this))
      ->mainControl->setMessage("T_VerNum_00", u"CTGP");
}

ui::UIControl *raceViewMainControl = NULL;
DECL_CXX_HOOK(int, ui::Page_Race::bindWindowLayout_, ui::UIPage *_this,
              ui::UIControl *mainControl, void *raceWindow) {
  int ret = real(_this, mainControl, raceWindow);
  raceViewMainControl = mainControl;
  raceViewMainControl->findPane("L_Speedometer_00")->setVisible(false);
  return ret;
}

DECL_CXX_HOOK(int, nn::nex::JobCafeLogin::NexAuthenticationFunc, void *_this,
              void *arg) {
  int r = real(_this, arg);
  // ACTNexAuthenticationResult* nexAuthResult =
  // (ACTNexAuthenticationResult*)(0x10210940 + MK8_DATA_OFFSET);
  // strcpy((char*)&nexAuthResult->m_NexIPAddress, "192.168.1.42");
  // nexAuthResult->m_Port = 1223;
  return r;
}

int MainThread(int argc, void *argv) {

  InitializePatcher();
  InitializePatchFilesystem();

  OSSleepTicks(OSMillisecondsToTicks(1500));

  ctgp::GlobalPatch::createInstance();
  InitializePatchMessage();
  ctgp::CoursePatch::createInstance();
  ctgp::OnlinePatch::createInstance();

  ctgp::GlobalPatchInstance->RegisterAttachRedirection(
      ui::Page_TitleTop::ID, Page_TitleTop_AttachementRedirection);
  MK8_HOOK_CXX_FUNC(0x0263FFC8, ui::Page_Race::bindWindowLayout_);
  MK8_HOOK_CXX_FUNC(0x0299C9E0, nn::nex::JobCafeLogin::NexAuthenticationFunc);

  OSSleepTicks(OSSecondsToTicks(8));
  sys::SystemEngine *engine = sys::SystemEngine::getEngine();

  ctgp::GlobalPatchInstance->m_IsRgsConnected = false;

  int cnt = 0;
  while (1) {

    if (errorArg.errorType == 1) {
      engine->launchErrorViewer(&errorArg);
      while (1) {
        GX2WaitForVsync();
      }
    }

    /* Speedometer text update */
    if (*object::spRaceDirector && // if race director exists
        *(uint32_t *)((*object::spRaceDirector) +
                      0x238) &&  // and if the race state ptr was allocated
        object::IsRaceState() && // and if we're in a race
        raceViewMainControl)     // and raceViewControl was created
    {
      object::KartDirector *kartDirector =
          (object::KartDirector *)*object::spKartDirector;
      float speed =
          kartDirector->getKartUnit(0)->vehicle->vehicleMove->driveSpd * 12.0f;
      char buffer[32];
      __os_snprintf(buffer, 32, "%.0f km/h", speed);
      char16_t *wbuffer = fromUTF8(buffer);
      ModifyMessage(wbuffer, SPEEDOMETER_MSG_ID, true);
      nw::lyt::Pane *speedo = raceViewMainControl->findPane("L_Speedometer_00");
      if (speedo) {
        speedo->setVisible(true);
        raceViewMainControl->setMessage(speedo, SPEEDOMETER_MSG_ID);
      }
    }

    GX2WaitForVsync();
    cnt++;
  }

  return 0;
}