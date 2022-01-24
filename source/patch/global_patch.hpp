#ifndef _PATCH_GLB_H
#define _PATCH_GLB_H

#include <mk8/ui/Control_Button.hpp>
#include <mk8/ui/UIEngine.hpp>
#include <mk8/ui/UIEvent.hpp>
#include <mk8/ui/UIPage.hpp>

#include <vector>

#include "utils.hpp"
#include "wiiu/debug.hpp"
#include "wiiu/patcher.h"

#define GLOBAL_PAGE_ID ((int)-1337)

typedef struct PageManagerAttachRedirect {
    int m_pageId = -1;
    void (*m_RedirectFunc)(ui::UIPage*) = NULL;

    PageManagerAttachRedirect(int pageId, void (*f)(ui::UIPage*)) {
        m_pageId = pageId;
        m_RedirectFunc = f;
    }

} PageManagerAttachRedirect;

typedef struct ControlButtonHandlerRedirect {
    int m_pageId = -1;
    bool (*m_RedirectFunc)(ui::Control_Button*, ui::UIEvent*) = NULL;

    ControlButtonHandlerRedirect(int pageId, bool (*f)(ui::Control_Button*, ui::UIEvent*)) {
        m_pageId = pageId;
        m_RedirectFunc = f;
    }

} ControlButtonHandlerRedirect;

typedef struct InputHandlerRedirect {

    bool (*m_RedirectFunc)(ui::UIInput*, int, int, uint32_t) = NULL;
    InputHandlerRedirect(bool (*f)(ui::UIInput*, int, int, uint32_t)) {
        m_RedirectFunc = f;
    }

} InputHandlerRedirect;

namespace ctgp {

class GlobalPatch : ctgp::common::RootClass {
  private:
    bool m_OnlineToggle = false;

  public:
    std::vector<PageManagerAttachRedirect> m_PageAttachRedirections;
    std::vector<ControlButtonHandlerRedirect> m_ButtonHandlerRedirections;
    std::vector<InputHandlerRedirect> m_InputHandlerRedirections;

    int m_LastRegion = -1;
    bool m_IsRgsConnected = false;

  public:
    GlobalPatch();

    /**
     * @brief Initializer
     *
     */
    virtual void Initialize();

    /**
     * @brief Register one attach redirection structure, it will be used in the
     * following attach calls if the conditions matches
     *
     * @param attachRedirection Reference to a "PageManagerAttachRedirect"
     * structure
     */
    void RegisterAttachRedirection(PageManagerAttachRedirect& attachRedirection) {
        m_PageAttachRedirections.push_back(attachRedirection);
    }

    /**
     * @brief Register one attach redirection structure, it will be used in the
     * following attach calls if the conditions matches
     *
     * @param pageId Page ID
     * @param f Function pointer
     */
    void RegisterAttachRedirection(int pageId, void (*f)(ui::UIPage*)) {
        m_PageAttachRedirections.push_back(PageManagerAttachRedirect(pageId, f));
    }

    /**
     * @brief Register one button handler redirection structure, it will be used
     * in the following handler calls if the conditions matches
     *
     * @param buttonRedirection Reference to a "PageManagerAttachRedirect"
     * structure
     */
    void RegisterButtonHandlerRedirection(ControlButtonHandlerRedirect& buttonRedirection) {
        m_ButtonHandlerRedirections.push_back(buttonRedirection);
    }

    /**
     * @brief Register one button handler redirection structure, it will be used
     * in the following handler calls if the conditions matches
     *
     * @param pageId Page ID
     * @param f Function pointer
     */
    void RegisterButtonHandlerRedirection(int pageId, bool (*f)(ui::Control_Button*, ui::UIEvent*)) {
        m_ButtonHandlerRedirections.push_back(ControlButtonHandlerRedirect(pageId, f));
    }

    /**
     * @brief Register one input handler redirection structure, it will be used in
     * the following handler calls if the conditions matches
     *
     * @param buttonRedirection Reference to a "InputHandlerRedirect" structure
     */
    void RegisterInputHandlerRedirection(InputHandlerRedirect& inputRedirection) {
        m_InputHandlerRedirections.push_back(inputRedirection);
    }

    /**
     * @brief Register one input handler redirection structure, it will be used in
     * the following handler calls if the conditions matches
     *
     * @param f Function pointer
     */
    void RegisterInputHandlerRedirection(bool (*f)(ui::UIInput*, int, int, uint32_t)) {
        m_InputHandlerRedirections.push_back(InputHandlerRedirect(f));
    }

    /**
     * @brief Set the Online Toggle member
     *
     * @param val Boolean
     */
    void setOnlineToggle(bool val) {
        m_OnlineToggle = val;
    }

    /**
     * @brief Get the Online Toggle member
     *
     * @return Boolean
     */
    bool getOnlineToggle() {
        return m_OnlineToggle;
    }

    /**
     * @brief Create a Instance object of GlobalPatch
     *
     * @return ctgp::CUP0*
     */
    static inline ctgp::GlobalPatch* createInstance() {
        ctgp::GlobalPatch* inst = new ctgp::GlobalPatch();
        inst->Initialize();
        return inst;
    }
};

///! CUP0 instance
extern GlobalPatch* GlobalPatchInstance;
} // namespace ctgp

#endif