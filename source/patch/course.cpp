#include "course.hpp"

uint8_t* leftTexture = nullptr;
nw::lyt::TextureInfo* leftTextureInfo = nullptr;

uint8_t* rightTexture = nullptr;
nw::lyt::TextureInfo* rightTextureInfo = nullptr;

ctgp::CoursePatch* ctgp::CoursePatchInstance;

DECL_CXX_HOOK(int, ui::Page_Course::LoadPanes, ui::Page_Course* page, ui::UIControl* mainControl, char r5) {

    if (!ctgp::CoursePatchInstance->m_ButtonHolders.contains(page->pageID)) {
        CupButtonHolder buttonHolder;
        buttonHolder.controlL1 = new ui::Control_CupButton(1);
        buttonHolder.controlL2 = new ui::Control_CupButton(1);
        buttonHolder.controlR1 = new ui::Control_CupButton(1);
        buttonHolder.controlR2 = new ui::Control_CupButton(1);
        ctgp::CoursePatchInstance->m_ButtonHolders.insert(std::make_pair(page->pageID, buttonHolder));
    }

    ui::Control_CupButton* controlL1 = ctgp::CoursePatchInstance->m_ButtonHolders[page->pageID].controlL1;
    ui::Control_CupButton* controlL2 = ctgp::CoursePatchInstance->m_ButtonHolders[page->pageID].controlL2;

    ui::Control_CupButton* controlR1 = ctgp::CoursePatchInstance->m_ButtonHolders[page->pageID].controlR1;
    ui::Control_CupButton* controlR2 = ctgp::CoursePatchInstance->m_ButtonHolders[page->pageID].controlR2;

    char** cupPaneNames = (char**)(0x100DC77C + MK8_DATA_OFFSET);

    if (page->pageID == 43) {

        controlL1->cupId = 12;
        controlL2->cupId = 12;
        controlR1->cupId = 13;
        controlR2->cupId = 13;

        page->createControl_(controlL1, mainControl, "L_CupIcon_12");

        for (int i = 1; i < 7; i++) {
            ui::Control_CupButton* cupBtn = new ui::Control_CupButton(1);
            page->createControl_(cupBtn, mainControl, cupPaneNames[i - 1]);
            page->cupIcons[i - 1] = cupBtn;
            cupBtn->field_00 = i;
        }

        page->createControl_(controlR1, mainControl, "L_CupIcon_13");
        page->createControl_(controlL2, mainControl, "L_CupIcon_12");

        for (int i = 9; i < 15; i++) {
            ui::Control_CupButton* cupBtn = new ui::Control_CupButton(1);
            page->createControl_(cupBtn, mainControl, cupPaneNames[i - 3]);
            page->cupIcons[i - 3] = cupBtn;
            cupBtn->field_00 = i;
        }

        page->createControl_(controlR2, mainControl, "L_CupIcon_13");

        controlL1->field_00 = 0;
        controlR1->field_00 = 7;
        controlL2->field_00 = 8;
        controlR2->field_00 = 15;

        for (int i = 0; i < 4; i++) {
            ui::Control_CourseButton* btn = new ui::Control_CourseButton(r5);
            char text[19] = "L_CourseSlctBtn_00";
            text[17] = '0' + i;
            page->createControl_(btn, mainControl, text);
            page->courseBtn[i] = btn;
            page->courseBtn[i]->field_00 = 12 + i;
        }
    } else {

        real(page, mainControl, r5);

        page->createControl_(controlL1, mainControl, "L_CupIcon_12");
        page->createControl_(controlR1, mainControl, "L_CupIcon_13");
        page->createControl_(controlL2, mainControl, "L_CupIcon_12");
        page->createControl_(controlR2, mainControl, "L_CupIcon_13");

        controlL1->field_00 = 16;
        controlR1->field_00 = 17;
        controlL2->field_00 = 18;
        controlR2->field_00 = 19;
    }

    uint32_t outSize = 0;

    leftTexture = GetArchiveFile("/vol/content/custom/ym_CupIconLeft^h.bflim", &outSize, 0x1000);
    leftTextureInfo = (nw::lyt::TextureInfo*)calloc(1, sizeof(nw::lyt::TextureInfo));
    nw::lyt::LoadTexture(leftTextureInfo, leftTexture, outSize);

    rightTexture = GetArchiveFile("/vol/content/custom/ym_CupIconRight^h.bflim", &outSize, 0x1000);
    rightTextureInfo = (nw::lyt::TextureInfo*)calloc(1, sizeof(nw::lyt::TextureInfo));
    nw::lyt::LoadTexture(rightTextureInfo, rightTexture, outSize);

    controlL1->setTexture(controlL1->findPane("P_CupIcon_00"), leftTextureInfo, 1);
    controlL2->setTexture(controlL2->findPane("P_CupIcon_00"), leftTextureInfo, 1);

    controlR1->setTexture(controlR1->findPane("P_CupIcon_00"), rightTextureInfo, 1);
    controlR2->setTexture(controlR2->findPane("P_CupIcon_00"), rightTextureInfo, 1);

    memcpy(ctgp::CUP0Instance->trackNamesMod, ctgp::CUP0Instance->trackNames, sizeof(ctgp::CUP0Instance->trackNamesMod));
    memcpy(ctgp::CUP0Instance->cupMessageIdSave, ctgp::CUP0Instance->cupMessageId, sizeof(ctgp::CUP0Instance->cupMessageIdSave));

    return 1;
}

void* cupIconsBflimData[12] = { 0 };
const char* cupNamesInternal[] = { "Mushroom", "Flower", "Star", "Special", "Shell", "Banana", "Leaf", "Thunder", "DLC02", "DLC03", "DLC04", "DLC05" };
void loadPage() {
    if (ctgp::CoursePatchInstance->m_CurrentPageNum == 0) {
        memcpy(ctgp::CUP0Instance->cupMessageId, ctgp::CUP0Instance->cupMessageIdSave, sizeof(ctgp::CUP0Instance->cupMessageIdSave));
        for (int i = 0; i < 8; i++) {
            ctgp::CUP0Instance->cupBflimFile[i]->texture.surface.image_data = ctgp::CUP0Instance->cupBflimFileSave[i].texture.surface.image_data;
            ctgp::CUP0Instance->cupBflimFile[i]->texture.surface.swizzle = ctgp::CUP0Instance->cupBflimFileSave[i].texture.surface.swizzle;
        }
    } else {
        int pageId = ctgp::CoursePatchInstance->m_CurrentPageNum - 1;
        for (int i = 0; i < 8; i++) {
            if (ctgp::CUP0Instance->pages[pageId]->pageCustomCups[i]) {
                ctgp::CUP0Instance->cupBflimFile[i]->texture.surface.image_data = ctgp::CUP0Instance->pages[pageId]->cupBflimFile[i]->texture.surface.image_data;
                ctgp::CUP0Instance->cupBflimFile[i]->texture.surface.swizzle = ctgp::CUP0Instance->pages[pageId]->cupBflimFile[i]->texture.surface.swizzle;
            }
        }
    }
}

void decrement_page() {

    if (ctgp::CoursePatchInstance->m_CurrentPageNum == 0)
        ctgp::CoursePatchInstance->m_CurrentPageNum = ctgp::CUP0Instance->numPages;
    else
        ctgp::CoursePatchInstance->m_CurrentPageNum--;

    loadPage();
}

void increment_page() {

    if (ctgp::CoursePatchInstance->m_CurrentPageNum == (int)ctgp::CUP0Instance->numPages)
        ctgp::CoursePatchInstance->m_CurrentPageNum = 0;
    else
        ctgp::CoursePatchInstance->m_CurrentPageNum++;

    loadPage();
}

void ctgp::CoursePatch::LoadPage() {
    loadPage();
}

/*

Cursor input:

0 0010 0000 (32) -> Up
0 0100 0000 (64) -> Bottom
0 1000 0000 (128) -> Left
1 0000 0000 (256) -> Right

*/

DECL_CXX_HOOK(int, ui::UICursorLink::next, ui::UICursorLink* link, unsigned int input, int currentPos) {

    if (ui::Page_CourseGP::getPage()->cursor->cursorLink == link) {
        link->field_08 = 8;
        if (input == 128 && currentPos == 0) {
            decrement_page();
            return -1;
        } else if (input == 256 && currentPos == 7) {
            increment_page();
            return -1;
        } else if (input == 256 && currentPos == 15) {
            increment_page();
            return -1;
        } else if (input == 128 && currentPos == 8) {
            decrement_page();
            return -1;
        }
    }

    return real(link, input, currentPos);
}

std::vector<std::tuple<const char*, nw::lyt::TextureInfo*>> customBattleTracks;
std::vector<ui::Control_CourseButton> backupPageBattle;
void handle_battle_swap() {

    ui::Page_CourseBattle* page = ui::Page_CourseBattle::getPage();

    if (ctgp::CoursePatchInstance->m_CustomBattle && backupPageBattle.size() != 8) {
        backupPageBattle.clear();
        for (int i = 0; i < 8; i++) {
            backupPageBattle.push_back(*page->m_CourseButtons[i]);
        }
    }

    if (ctgp::CoursePatchInstance->m_CustomBattle) {
        for (int i = 0; i < 8; i++) {
            DBG_LOG("%d: %d %s\n", i, page->m_CourseButtons[i]->courseId, ctgp::CUP0Instance->trackNames[page->m_CourseButtons[i]->courseId]);
            const char* path;
            nw::lyt::TextureInfo* textureInfo;
            std::tie(path, textureInfo) = customBattleTracks[i];
            ui::Control_CourseButton* button = page->m_CourseButtons[i];
            button->setTexture(button->coursePictPane, textureInfo, 1);
            button->setVisible(button->coursePictPane, 1);
            button->setVisible(button->courseMoviePane, 0);
            button->setMessage(button->courseNameTextPane, DELUXE_RETRO_BT_IDs + i);
            button->setVisible(button->classicNameTextPane, 1);
            button->setMessage(button->classicNameTextPane, DELUXE_RETRO_MSG_ID);
            ctgp::CUP0Instance->trackNamesMod[button->courseId] = (char*)path;
        }
    }

    if (!ctgp::CoursePatchInstance->m_CustomBattle) {
        for (int i = 0; i < 8; i++) {
            *page->m_CourseButtons[i] = backupPageBattle[i];
            ctgp::CUP0Instance->trackNamesMod[page->m_CourseButtons[i]->courseId] = nullptr;

            ui::Heap_Common* heap = ui::Heap_Common::getHeap();
            for (int cupIdx = 0; cupIdx < 12; cupIdx++) {
                for (int trackIdx = 0; trackIdx < 8; trackIdx++) {
                    if (heap->courseData->courseList.cups[cupIdx].courses[trackIdx].id == page->m_CourseButtons[i]->courseId) {
                        page->m_CourseButtons[i]->setCourse(&heap->courseData->courseList.cups[cupIdx].courses[trackIdx], nullptr);
                        break;
                    }
                }
            }
        }
    }
}

DECL_CXX_HOOK(int, ui::CursorLink_CourseMatrix::next, ui::CursorLink_CourseMatrix* _this, uint32_t input, int currentPos) {

    if (ui::Page_CourseBattle::getPage()->cursor->cursorLink == _this) {
        if (input == 128 && currentPos == 0) {
            ctgp::CoursePatchInstance->m_CustomBattle = !ctgp::CoursePatchInstance->m_CustomBattle;
            handle_battle_swap();
            return -1;
        } else if (input == 256 && currentPos == 5) {
            ctgp::CoursePatchInstance->m_CustomBattle = !ctgp::CoursePatchInstance->m_CustomBattle;
            handle_battle_swap();
            return -1;
        } else if (input == 256 && currentPos == 11) {
            ctgp::CoursePatchInstance->m_CustomBattle = !ctgp::CoursePatchInstance->m_CustomBattle;
            handle_battle_swap();
            return -1;
        } else if (input == 128 && currentPos == 6) {
            ctgp::CoursePatchInstance->m_CustomBattle = !ctgp::CoursePatchInstance->m_CustomBattle;
            handle_battle_swap();
            return -1;
        }
    }

    return real(_this, input, currentPos);
}

void changeCourseButtons(ui::Page_Course* page, int cupId) {

    ui::Heap_Common* heap = ui::Heap_Common::getHeap();
    if (ctgp::CoursePatchInstance->m_CurrentPageNum > 0 && cupId < 8 && ctgp::CUP0Instance->pages[ctgp::CoursePatchInstance->m_CurrentPageNum - 1]->pageCustomCups[cupId]) {
        for (int i = 0; i < 4; i++) {
            page->courseBtn[i]->setTexture(page->courseBtn[i]->coursePictPane,
                                           ctgp::CUP0Instance->pages[ctgp::CoursePatchInstance->m_CurrentPageNum - 1]->trackBflimFiles[cupId][i], 1);
            page->courseBtn[i]->setVisible(page->courseBtn[i]->coursePictPane, 1);
            page->courseBtn[i]->setVisible(page->courseBtn[i]->courseMoviePane, 0);
            page->courseBtn[i]->setMessage(page->courseBtn[i]->courseNameTextPane,
                                           ctgp::CUP0Instance->pages[ctgp::CoursePatchInstance->m_CurrentPageNum - 1]->trackNames[cupId][i]);
            if (ctgp::CUP0Instance->pages[ctgp::CoursePatchInstance->m_CurrentPageNum - 1]->retroTrackNames[cupId][i] > 0) {
                page->courseBtn[i]->setVisible(page->courseBtn[i]->classicNameTextPane, 1);
                page->courseBtn[i]->setMessage(page->courseBtn[i]->classicNameTextPane,
                                               ctgp::CUP0Instance->pages[ctgp::CoursePatchInstance->m_CurrentPageNum - 1]->retroTrackNames[cupId][i]);
            } else {
                page->courseBtn[i]->setVisible(page->courseBtn[i]->classicNameTextPane, 0);
            }
            page->mainControl->setMessage(page->cupNameTextPane, ctgp::CUP0Instance->pages[ctgp::CoursePatchInstance->m_CurrentPageNum - 1]->cupName[cupId]);
            ctgp::CUP0Instance->trackNamesMod[heap->courseData->courseList.cups[cupId].courses[i].id] =
                ctgp::CUP0Instance->pages[ctgp::CoursePatchInstance->m_CurrentPageNum - 1]->trackPaths[cupId][i];
        }
    }

    if (ctgp::CoursePatchInstance->m_CurrentPageNum == 0) {
        for (int i = 0; i < 4; i++) {
            ctgp::CUP0Instance->trackNamesMod[heap->courseData->courseList.cups[cupId].courses[i].id] = nullptr;
        }
    }
}

DECL_CXX_HOOK(int, ui::Page_CourseBattle::onHandler, ui::Page_CourseBattle* _this, ui::UIEvent* event) {

    for (int i = 0; i < 8; i++) {
        if (_this->m_CourseButtons[i] == event->cursor_event.m_SourceControl) {
            event->cursor_event.m_ControlId = i;
            break;
        }
    }

    return real(_this, event);
}

DECL_CXX_HOOK(int, ui::Page_CourseGP::onHandler, ui::Page_CourseGP* _this, ui::UIEvent* event) {

    for (int i = 0; i < 12; i++) {
        if (_this->cupIcons[i] == event->cursor_event.m_SourceControl) {
            event->cursor_event.m_ControlId = i;
            break;
        }
    }

    int ret = real(_this, event);
    changeCourseButtons(_this, ui::Heap_Common::getHeap()->courseData->courseRelatedData.currentlySelectedCup);
    return ret;
}

DECL_CXX_HOOK(int, ui::Page_Course44::onHandler, ui::Page_Course* _this, ui::UIEvent* event) {
    int ret = real(_this, event);
    changeCourseButtons(_this, ui::Heap_Common::getHeap()->courseData->courseRelatedData.currentlySelectedCup);
    return ret;
}

DECL_CXX_HOOK(int, ui::Page_Course63::onHandler, ui::Page_Course* _this, ui::UIEvent* event) {
    int ret = real(_this, event);
    changeCourseButtons(_this, ui::Heap_Common::getHeap()->courseData->courseRelatedData.currentlySelectedCup);
    return ret;
}

DECL_CXX_HOOK(void*, nw::lyt::MultiArcResourceAccessor::GetResource, nw::lyt::MultiArcResourceAccessor* _this, uint32_t resId, const char* path, uint32_t* out_size) {
    void* resource = real(_this, resId, path, out_size);
    if (resId == 0x74696D67) {
        if (strstr(path, "CupIcon")) {
            char* outName = (char*)calloc(1, 256);
            sscanf(path, "ym_CupIcon%s^h.bflim", outName);
            char* offset = strstr(outName, "^h.bflim");
            if (!offset) {
                free(outName);
                return resource;
            }
            outName[offset - outName] = 0;
            for (int i = 0; i < 12; i++) {
                if (strcmp(cupNamesInternal[i], outName) == 0) {
                    cupIconsBflimData[i] = resource;
                    free(outName);
                    return resource;
                }
            }
        }
    }

    return resource;
}

DECL_CXX_HOOK(int, nw::lyt::LoadTexture, nw::lyt::TextureInfo* textureInfo, const void* filePtr, uint32_t size) {
    int ret = real(textureInfo, filePtr, size);
    for (int i = 0; i < 12; i++) {
        if (cupIconsBflimData[i] == filePtr) {
            ctgp::CUP0Instance->cupBflimFile[i] = textureInfo;
            memcpy(&ctgp::CUP0Instance->cupBflimFileSave[i], textureInfo, sizeof(nw::lyt::TextureInfo));
            break;
        }
    }
    return ret;
}

bool Global_ButtonHandler(ui::Control_Button* _this, ui::UIEvent* event) {
    for (auto& [pageId, ent] : ctgp::CoursePatchInstance->m_ButtonHolders) {
        if (pageId == _this->ownerPage->pageID) {
            if (_this == ent.controlL1 || _this == ent.controlL2 || _this == ent.controlR1 || _this == ent.controlR2) {
                audio::utl::startSound2D("SE_SYS_CMN_CURSOR");
                if (event->m_InputValue <= 1) {
                    return true;
                }
                break;
            }
        }
    }

    return false;
}

/*

Cursor input:

0 0010 0000 (32) -> Up
0 0100 0000 (64) -> Bottom
0 1000 0000 (128) -> Left
1 0000 0000 (256) -> Right

*/

DECL_CXX_HOOK(int, ui::UICursor::next_, ui::UICursor* _this, int r4, int r5, unsigned int input) {

    int next = real(_this, r4, r5, input);

    ui::UIPageManager* pageMgr = ui::UIEngine::getSpInstance()->pageManager;
    for (int j = 0; j < pageMgr->layouts.mSize; j++) {
        if (pageMgr->layouts.mBuffer[j] && pageMgr->layouts.mBuffer[j]->cursor == _this &&
            (pageMgr->layouts.mBuffer[j]->pageID == 44 || (pageMgr->layouts.mBuffer[j]->pageID == 63 && ctgp::GlobalPatchInstance->getOnlineToggle()))) {

            if (_this->playerCursors[0].cursorPosition == 0 && input == 128)
                return 16;
            else if (_this->playerCursors[0].cursorPosition == 16 && input == 256)
                return 0;

            if (_this->playerCursors[0].cursorPosition == 6 && input == 128)
                return 18;
            else if (_this->playerCursors[0].cursorPosition == 18 && input == 256)
                return 6;

            if (_this->playerCursors[0].cursorPosition == 5 && input == 256)
                return 17;
            else if (_this->playerCursors[0].cursorPosition == 17 && input == 128)
                return 5;

            if (_this->playerCursors[0].cursorPosition == 11 && input == 256)
                return 19;
            else if (_this->playerCursors[0].cursorPosition == 19 && input == 128)
                return 11;

            if (input == 128 && _this->playerCursors[0].cursorPosition == 16) {
                decrement_page();
                return -1;
            } else if (input == 256 && _this->playerCursors[0].cursorPosition == 17) {
                increment_page();
                return -1;
            } else if (input == 256 && _this->playerCursors[0].cursorPosition == 19) {
                increment_page();
                return -1;
            } else if (input == 128 && _this->playerCursors[0].cursorPosition == 18) {
                decrement_page();
                return -1;
            }
        }
    }

    return next;
}

ui::Control_Button* changeBattlePageBtn = nullptr;
DECL_CXX_HOOK(void, ui::Page_CourseBattle::onCreate, ui::Page_CourseBattle* _this) {

    if (!ctgp::CoursePatchInstance->m_ButtonHolders.contains(_this->pageID)) {
        CupButtonHolder buttonHolder;
        buttonHolder.controlL1 = new ui::Control_CupButton(1);
        buttonHolder.controlL2 = new ui::Control_CupButton(1);
        buttonHolder.controlR1 = new ui::Control_CupButton(1);
        buttonHolder.controlR2 = new ui::Control_CupButton(1);
        ctgp::CoursePatchInstance->m_ButtonHolders.insert(std::make_pair(_this->pageID, buttonHolder));
    }

    ui::Control_CupButton* controlL1 = ctgp::CoursePatchInstance->m_ButtonHolders[_this->pageID].controlL1;
    ui::Control_CupButton* controlL2 = ctgp::CoursePatchInstance->m_ButtonHolders[_this->pageID].controlL2;

    ui::Control_CupButton* controlR1 = ctgp::CoursePatchInstance->m_ButtonHolders[_this->pageID].controlR1;
    ui::Control_CupButton* controlR2 = ctgp::CoursePatchInstance->m_ButtonHolders[_this->pageID].controlR2;

    controlL1->cupId = 12;
    controlL2->cupId = 12;
    controlR1->cupId = 13;
    controlR2->cupId = 13;

    ui::UIControl* bflyt = new ui::UIControl();
    _this->loadControl_(bflyt, 5, "mn_BattleCourseSlct_00.bflyt");

    _this->m_BattleCourseBflyt = bflyt;
    if (!bflyt->parentControl)
        bflyt->flags |= 0x100u;

    ui::UIAnimator* animator = _this->createAnimator(0, bflyt, 1);
    animator->bind(0, "RandomBtnOnOff");

    char control_str[] = "L_Course_00";

    _this->createControl_(controlL1, bflyt, "L_Course_08");

    for (int i = 0; i < 4; i++) {
        control_str[10] = '0' + i;
        ui::Control_CourseButton* button = new ui::Control_CourseButton(0);
        button->flow = (ui::UIFlow*)&_this->m_FlowOpen0;
        button->field_00 = i + 1;
        _this->createControl_(button, bflyt, control_str);
        _this->m_CourseButtons[i] = button;
    }

    _this->createControl_(controlR1, bflyt, "L_Course_09");
    _this->createControl_(controlL2, bflyt, "L_Course_08");

    for (int i = 4; i < 8; i++) {
        control_str[10] = '0' + i;
        ui::Control_CourseButton* button = new ui::Control_CourseButton(0);
        button->flow = (ui::UIFlow*)&_this->m_FlowOpen0;
        button->field_00 = i + 3;
        _this->createControl_(button, bflyt, control_str);
        _this->m_CourseButtons[i] = button;
    }

    _this->createControl_(controlR2, bflyt, "L_Course_09");

    controlL1->field_00 = 0;
    controlR1->field_00 = 5;
    controlL2->field_00 = 6;
    controlR2->field_00 = 11;

    // 6x2 matrix from the original 4x2
    _this->m_CourseMatrix = new ui::CursorLink_CourseMatrix();
    _this->m_CourseMatrix->m_SizeX = 6;
    _this->m_CourseMatrix->m_SizeY = 2;
    _this->m_CourseMatrix->field_1C = 0;

    void (*ui_UIControlSlider_push)(int _this, ui::UIControl* control) = (void (*)(int, ui::UIControl*))(0x025370A4 + MK8_TEXT_OFFSET);

    _this->cursor->setLink(_this->m_CourseMatrix);
    ui_UIControlSlider_push(_this->m_pUIControlSlider, bflyt);

    /* leftTexture and rightTexture guaranteed to be allocated and set since page
     * 43 onCreate happens before page 45 */
    controlL1->setTexture(controlL1->findPane("P_CupIcon_00"), leftTextureInfo, 1);
    controlL2->setTexture(controlL2->findPane("P_CupIcon_00"), leftTextureInfo, 1);

    controlR1->setTexture(controlR1->findPane("P_CupIcon_00"), rightTextureInfo, 1);
    controlR2->setTexture(controlR2->findPane("P_CupIcon_00"), rightTextureInfo, 1);
}

void ctgp::CoursePatch::Initialize() {

    AddMessage((char16_t*)u"DX", DELUXE_RETRO_MSG_ID);

    AddMessage((char16_t*)u"Battle Stadium", DELUXE_RETRO_BT_IDs + 0);
    AddMessage((char16_t*)u"Sweet Sweet Kingdom", DELUXE_RETRO_BT_IDs + 1);
    AddMessage((char16_t*)u"Dragon Palace", DELUXE_RETRO_BT_IDs + 2);
    AddMessage((char16_t*)u"Lunar Colony", DELUXE_RETRO_BT_IDs + 3);
    AddMessage((char16_t*)u"Wuhu Town", DELUXE_RETRO_BT_IDs + 4);
    AddMessage((char16_t*)u"Luigi's Mansion", DELUXE_RETRO_BT_IDs + 5);
    AddMessage((char16_t*)u"Battle Course 1", DELUXE_RETRO_BT_IDs + 6);
    AddMessage((char16_t*)u"Urchin Underpass", DELUXE_RETRO_BT_IDs + 7);

    uint32_t outSize = 0;
    uint8_t* trackData = nullptr;
    nw::lyt::TextureInfo* textureInfo = nullptr;

    /* Battle Stadium entry */
    trackData = GetArchiveFile("/vol/content/custom/cPict_GDx_BtStadium.bflim", &outSize, 0x1000);
    textureInfo = (nw::lyt::TextureInfo*)calloc(1, sizeof(nw::lyt::TextureInfo));
    nw::lyt::LoadTexture(textureInfo, trackData, outSize);
    customBattleTracks.push_back(std::make_tuple("Gu_BtStadium", textureInfo));

    /* Sweet Sweet Kingdom entry */
    trackData = GetArchiveFile("/vol/content/custom/cPict_GDx_BtSweetKingdom.bflim", &outSize, 0x1000);
    textureInfo = (nw::lyt::TextureInfo*)calloc(1, sizeof(nw::lyt::TextureInfo));
    nw::lyt::LoadTexture(textureInfo, trackData, outSize);
    customBattleTracks.push_back(std::make_tuple("Gu_BtSweet", textureInfo));

    /* Dragon Palace entry */
    trackData = GetArchiveFile("/vol/content/custom/cPict_GDx_BtDragonPalace.bflim", &outSize, 0x1000);
    textureInfo = (nw::lyt::TextureInfo*)calloc(1, sizeof(nw::lyt::TextureInfo));
    nw::lyt::LoadTexture(textureInfo, trackData, outSize);
    customBattleTracks.push_back(std::make_tuple("Gu_BtDojo", textureInfo));

    /* Lunar Colony entry */
    trackData = GetArchiveFile("/vol/content/custom/cPict_GDx_BtLunarColony.bflim", &outSize, 0x1000);
    textureInfo = (nw::lyt::TextureInfo*)calloc(1, sizeof(nw::lyt::TextureInfo));
    nw::lyt::LoadTexture(textureInfo, trackData, outSize);
    customBattleTracks.push_back(std::make_tuple("Gu_BtMoon", textureInfo));

    /* Wuhu Town entry */
    trackData = GetArchiveFile("/vol/content/custom/cPict_GDx_BtWuhuTown.bflim", &outSize, 0x1000);
    textureInfo = (nw::lyt::TextureInfo*)calloc(1, sizeof(nw::lyt::TextureInfo));
    nw::lyt::LoadTexture(textureInfo, trackData, outSize);
    customBattleTracks.push_back(std::make_tuple("Gu_BtWuhuTown", textureInfo));

    /* Luigi's Mansion entry */
    trackData = GetArchiveFile("/vol/content/custom/cPict_GDx_BtLuigiMansion.bflim", &outSize, 0x1000);
    textureInfo = (nw::lyt::TextureInfo*)calloc(1, sizeof(nw::lyt::TextureInfo));
    nw::lyt::LoadTexture(textureInfo, trackData, outSize);
    customBattleTracks.push_back(std::make_tuple("Gu_BtLuigiMansion", textureInfo));

    /* Battle Course 1 entry */
    trackData = GetArchiveFile("/vol/content/custom/cPict_GDx_BtBattleCourse1.bflim", &outSize, 0x1000);
    textureInfo = (nw::lyt::TextureInfo*)calloc(1, sizeof(nw::lyt::TextureInfo));
    nw::lyt::LoadTexture(textureInfo, trackData, outSize);
    customBattleTracks.push_back(std::make_tuple("Gu_BtBattle1", textureInfo));

    /* Urchin Underpass entry */
    trackData = GetArchiveFile("/vol/content/custom/cPict_GDx_BtUrchin.bflim", &outSize, 0x1000);
    textureInfo = (nw::lyt::TextureInfo*)calloc(1, sizeof(nw::lyt::TextureInfo));
    nw::lyt::LoadTexture(textureInfo, trackData, outSize);
    customBattleTracks.push_back(std::make_tuple("Gu_BtDekaLine", textureInfo));

    ctgp::CUP0::createInstance();
    ctgp::GlobalPatchInstance->RegisterButtonHandlerRedirection(GLOBAL_PAGE_ID, Global_ButtonHandler);

    MK8_HOOK_CXX_FUNC(0x025B9680, ui::Page_Course::LoadPanes);
    MK8_HOOK_CXX_FUNC(0x02538B2C, ui::UICursorLink::next);
    MK8_HOOK_CXX_FUNC(0x025BD0E0, ui::Page_CourseBattle::onHandler);
    MK8_HOOK_CXX_FUNC(0x025BBD18, ui::Page_CourseGP::onHandler);
    MK8_HOOK_CXX_FUNC(0x025BC24C, ui::Page_Course44::onHandler);
    MK8_HOOK_CXX_FUNC(0x025FDCF8, ui::Page_Course63::onHandler);
    MK8_HOOK_CXX_FUNC(0x02694B70, nw::lyt::MultiArcResourceAccessor::GetResource);
    MK8_HOOK_CXX_FUNC(MK8_NW_LYT_LOAD_TEXTURE, nw::lyt::LoadTexture);
    MK8_HOOK_CXX_FUNC(0x02537CE0, ui::UICursor::next_);
    MK8_HOOK_CXX_FUNC(0x02544F10, ui::CursorLink_CourseMatrix::next);
    MK8_HOOK_CXX_FUNC(0x025BC8D8, ui::Page_CourseBattle::onCreate);

    Write32((uint32_t*)(0x025BBB9C + 0x0C180000), PowerPC_LI(gpr11, 8));
    Write32((uint32_t*)(0x025BC3BC + 0x0C180000), PowerPC_LI(gpr5, 14));

    CoursePatchInstance = this;
}