#include "Engine/Engine.h"
#include "Engine/EngineGlobals.h"
#include "Engine/Localization.h"
#include "Engine/Time.h"
#include "Engine/Graphics/IRender.h"

#include "GUI/UI/UIModal.h"


CURRENT_SCREEN modal_window_prev_screen_type;

GUIWindow_Modal::GUIWindow_Modal(const char *message, UIMessageType on_release_event)
    : GUIWindow(WINDOW_ModalWindow, {0, 0}, render->GetRenderDimensions(), on_release_event, message) {
    pEventTimer->Pause();
    prev_screen_type = current_screen_type;
    current_screen_type = CURRENT_SCREEN::SCREEN_MODAL_WINDOW;
}

void GUIWindow_Modal::Update() {
    GUIWindow pWindow;
    pWindow.Init();
    pWindow.sHint = StringPrintf("%s\n \n%s", pModalWindow->sHint.c_str(),
                                 localization->GetString(LSTR_PRESS_ESCAPE));
    pWindow.uFrameWidth = 400;
    pWindow.uFrameHeight = 100;
    pWindow.uFrameX = 120;
    pWindow.uFrameY = 140;
    pWindow.uFrameZ = 519;
    pWindow.uFrameW = 239;
    pWindow.DrawMessageBox(0);
}

void GUIWindow_Modal::Release() {
    pMessageQueue_50CBD0->AddGUIMessage(static_cast<UIMessageType>(wData.val), 0, 0);

    current_screen_type = prev_screen_type;
    pEventTimer->Resume();

    GUIWindow::Release();
}
