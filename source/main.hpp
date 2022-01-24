#ifndef _PRGM_MAIN_H
#define _PRGM_MAIN_H

#include <malloc.h>
#include <stdint.h>
#include <stdlib.h>

#include "wiiu/iosu_kernel.h"
#include "wiiu/patcher.h"

#include "patch/course.hpp"
#include "patch/cup0.hpp"
#include "patch/global_patch.hpp"
#include "patch/message.hpp"
#include "patch/online.hpp"

#include <mk8/enl/Framework.hpp>
#include <mk8/enl/MatchingManager.hpp>
#include <mk8/nn/act/nex.hpp>
#include <mk8/nn/nex/MatchMakingExtension.hpp>
#include <mk8/object/Item/ItemDirector.hpp>
#include <mk8/object/Kart/KartDirector.hpp>
#include <mk8/object/Race/RaceDirector.hpp>
#include <mk8/sead/StringUtil/StringUtil.hpp>
#include <mk8/sys/SystemEngine.hpp>
#include <mk8/ui/Heaps/Heap_Common.hpp>
#include <mk8/ui/Pages/Page_CourseGP.hpp>
#include <mk8/ui/Pages/Page_TitleTop.hpp>
#include <mk8/ui/UICursor.hpp>
#include <mk8/ui/UIEngine.hpp>
#include <mk8/ui/UIEvent.hpp>

int MainThread(int argc, void* argv);

extern bool readyToExit;

#endif