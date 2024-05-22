#pragma once

#include "imgui.h"
#include "imgui_internal.h"

namespace raytracing {

/* Wrap ImGui widgets which return bool on change so we can register when the UI has changed. */
/* Providing `dirty` = false means a change won't be picked up for that widget. */
#define WRAP_IMGUI_BOOL_FN(fn)                                \
    template <class... Args>                                  \
    static bool fn(Args&&... args) {                          \
        bool change = ImGui::fn(std::forward<Args>(args)...); \
        s_DirtyFlag |= change;                                \
        return change;                                        \
    }                                                         \
    template <class... Args>                                  \
    static bool fn##ND(Args&&... args) { return ImGui::fn(std::forward<Args>(args)...); }
/* ND -> No Dirty, these versions of the functions don't register a UI change. */

class UI {
   private:
    /* True if any UI widget changed. Cleared by UI::Render */
    static bool s_DirtyFlag;

   public:
    static void Init(float scale);
    static void PreRender();
    static void Render();
    static void Shutdown();

    static bool WantInput();

    static bool HasChanged() { return s_DirtyFlag; }

    WRAP_IMGUI_BOOL_FN(Button);
    WRAP_IMGUI_BOOL_FN(Checkbox);
    WRAP_IMGUI_BOOL_FN(ColorEdit3);
    WRAP_IMGUI_BOOL_FN(InputFloat);
    WRAP_IMGUI_BOOL_FN(InputFloat3);
    WRAP_IMGUI_BOOL_FN(InputInt2);
    WRAP_IMGUI_BOOL_FN(Selectable);
    WRAP_IMGUI_BOOL_FN(SliderInt);
    WRAP_IMGUI_BOOL_FN(SliderFloat);
    WRAP_IMGUI_BOOL_FN(SliderFloat3);
};

}  // namespace raytracing