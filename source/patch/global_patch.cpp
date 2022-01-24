#include "global_patch.hpp"

ctgp::GlobalPatch* ctgp::GlobalPatchInstance = NULL;

DECL_CXX_HOOK(int, ui::UIPageManager::attach, ui::UIPageManager* _this, ui::UIPage* page, bool unk) {
    for (auto& ent : ctgp::GlobalPatchInstance->m_PageAttachRedirections) {
        if ((ent.m_pageId == GLOBAL_PAGE_ID) || (page->pageID == ent.m_pageId)) {
            ent.m_RedirectFunc(page);
        }
    }

    return real(_this, page, unk);
}

//! TODO: Crash prevention, set
DECL_CXX_HOOK(int, ui::Control_Button::onHandler, ui::Control_Button* _this, ui::UIEvent* event) {

    bool cancel = false;
    for (auto& ent : ctgp::GlobalPatchInstance->m_ButtonHandlerRedirections) {
        if ((ent.m_pageId == GLOBAL_PAGE_ID) || (_this->ownerPage->pageID == ent.m_pageId)) {
            if (ent.m_RedirectFunc(_this, event))
                cancel = true;
        }
    }

    if (cancel)
        return (int)_this;
    return real(_this, event);
}

DECL_CXX_HOOK(int, ui::UIInput::runHandler, ui::UIInput* _this, int issuer, int controllerIdx, uint32_t input) {

    bool cancel = false;
    for (auto& ent : ctgp::GlobalPatchInstance->m_InputHandlerRedirections) {
        if (ent.m_RedirectFunc(_this, issuer, controllerIdx, input))
            cancel = true;
    }

    if (cancel)
        return (int)_this;
    return real(_this, issuer, controllerIdx, input);
}

ctgp::GlobalPatch::GlobalPatch() {
    this->setName("ctgp::GlobalPatch");
}

void ctgp::GlobalPatch::Initialize() {
    MK8_HOOK_CXX_FUNC(0x0253FF98, ui::UIPageManager::attach);
    MK8_HOOK_CXX_FUNC(0x02578ADC, ui::Control_Button::onHandler);
    MK8_HOOK_CXX_FUNC(0x0253BD70, ui::UIInput::runHandler);
    GlobalPatchInstance = this;
}