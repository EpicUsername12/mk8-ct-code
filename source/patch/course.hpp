#ifndef _PATCH_COURSE_GP_H
#define _PATCH_COURSE_GP_H

#include <mk8/audio/utl/AudioUtil.hpp>
#include <mk8/net/NetworkTransporter_Menu.h>
#include <mk8/nn/pia.h>
#include <mk8/nw/lyt/Resource.hpp>
#include <mk8/sead/StringUtil/StringUtil.hpp>
#include <mk8/sys/SystemEngine.hpp>
#include <mk8/ui/Control_WifiCmnButton.hpp>
#include <mk8/ui/Heaps/Heap_Common.hpp>
#include <mk8/ui/Pages/Page_CourseBattle.hpp>
#include <mk8/ui/Pages/Page_CourseGP.hpp>
#include <mk8/ui/Pages/Page_TitleTop.hpp>
#include <mk8/ui/Pages/Page_WifiTop.hpp>
#include <mk8/ui/UICursor.hpp>
#include <mk8/ui/UIEngine.hpp>
#include <mk8/ui/UIEvent.hpp>

#include "patch/cup0.hpp"
#include "patch/global_patch.hpp"
#include "utils.hpp"
#include "wiiu/debug.hpp"
#include "wiiu/patcher.h"

#include <map>
#include <tuple>

struct CupButtonHolder {
    ui::Control_CupButton* controlL1 = NULL;
    ui::Control_CupButton* controlL2 = NULL;
    ui::Control_CupButton* controlR1 = NULL;
    ui::Control_CupButton* controlR2 = NULL;
};

#define DELUXE_RETRO_MSG_ID 25000
#define DELUXE_RETRO_BT_IDs 25001

namespace ctgp {

class CoursePatch : ctgp::common::RootClass {
  private:
  public:
    bool m_CustomBattle = false;
    int m_CurrentPageNum = 0;
    std::map<int, CupButtonHolder> m_ButtonHolders;

  public:
    /**
     * @brief Construct a new Course Patch object
     *
     */
    CoursePatch() {
        this->setName("ctgp::CoursePatch");
    }

    void LoadPage();

    /**
     * @brief Initialize the CoursePatch object
     *
     */
    virtual void Initialize();

    /**
     * @brief Create a Instance of CoursePatch
     *
     * @return ctgp::CoursePatch*
     */
    static inline ctgp::CoursePatch* createInstance() {
        ctgp::CoursePatch* inst = new ctgp::CoursePatch();
        inst->Initialize();
        return inst;
    }
};

extern CoursePatch* CoursePatchInstance;

} // namespace ctgp

#endif