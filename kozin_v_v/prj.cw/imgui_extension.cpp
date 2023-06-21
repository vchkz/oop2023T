#include "imgui_extension.h"
#include "imgui_internal.h"
#include <cmath>
#include <algorithm>


namespace ImGui {
bool ColorPicker(const char* label, float col[3])
{
    static const float CROSSHAIR_SIZE = 7.0f;
    static const ImVec2 HUE_PICKER_SIZE = ImVec2(200, 25);

    ImColor color(col[0], col[1], col[2]);
    bool value_changed = false;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImVec2 picker_pos = ImGui::GetCursorScreenPos();

    ImColor colors[] = { ImColor(255, 0, 0),
        ImColor(255, 255, 0),
        ImColor(0, 255, 0),
        ImColor(0, 255, 255),
        ImColor(0, 0, 255),
        ImColor(255, 0, 255),
        ImColor(255, 0, 0) };


    for (int i = 0; i < 6; ++i)
    {
        draw_list->AddRectFilledMultiColor(
            ImVec2(picker_pos.x + 10 + i * (HUE_PICKER_SIZE.x / 6), picker_pos.y ),
            ImVec2(picker_pos.x + 10 + (i + 1) * (HUE_PICKER_SIZE.x / 6),picker_pos.y + HUE_PICKER_SIZE.y),
            colors[i],
            colors[i + 1],
            colors[i + 1],
            colors[i]);
    }

    float hue, saturation, value;
    ImGui::ColorConvertRGBtoHSV(
        color.Value.x, color.Value.y, color.Value.z, hue, saturation, value);

    draw_list->AddLine(
        ImVec2(picker_pos.x + hue * HUE_PICKER_SIZE.x + 10, picker_pos.y - 2),
        ImVec2(picker_pos.x + hue * HUE_PICKER_SIZE.x + 10, picker_pos.y + HUE_PICKER_SIZE.y + 2),
        ImColor(255, 255, 255)
    );

    value = 1.0f;
    saturation = 1.0f;

    ImGui::SetCursorScreenPos(ImVec2(picker_pos.x, picker_pos.y));
    ImGui::InvisibleButton("hue_selector", ImVec2(HUE_PICKER_SIZE.x, HUE_PICKER_SIZE.y));

    if( (ImGui::IsItemHovered()||ImGui::IsItemActive()) && ImGui::GetIO().MouseDown[0])
    {
        ImVec2 mouse_pos_in_canvas = ImVec2(
            ImGui::GetIO().MousePos.x - picker_pos.x - 10, ImGui::GetIO().MousePos.y - picker_pos.y);

        /* Previous horizontal bar will represent hue=1 (bottom) as hue=0 (top). Since both colors are red, we clamp at (-2, above edge) to avoid visual continuities */
        /**/ if( mouse_pos_in_canvas.x <                     0 ) mouse_pos_in_canvas.x = 0;
        else if( mouse_pos_in_canvas.x >= HUE_PICKER_SIZE.x - 2 ) mouse_pos_in_canvas.x = HUE_PICKER_SIZE.x - 2;

        hue = mouse_pos_in_canvas.x / (HUE_PICKER_SIZE.x - 1 );
        value_changed = true;
    }

    color = ImColor::HSV(hue > 0 ? hue : 1e-6, saturation > 0 ? saturation : 1e-6, value > 0 ? value : 1e-6);
    col[0] = color.Value.x;
    col[1] = color.Value.y;
    col[2] = color.Value.z;
    return value_changed | ImGui::ColorEdit3(label, col);
}

bool Spinner(const char* label, float radius, int thickness, const ImU32& color) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size((radius )*2, (radius + style.FramePadding.y)*2);

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    // Render
    window->DrawList->PathClear();

    int num_segments = 30;
    int start = abs(ImSin(g.Time*1.8f)*(num_segments-5));

    const float a_min = IM_PI*2.0f * ((float)start) / (float)num_segments;
    const float a_max = IM_PI*2.0f * ((float)num_segments-3) / (float)num_segments;

    const ImVec2 centre = ImVec2(pos.x+radius, pos.y+radius+style.FramePadding.y);

    for (int i = 0; i < num_segments; i++) {
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a+g.Time*8) * radius,
                                            centre.y + ImSin(a+g.Time*8) * radius));
    }

    window->DrawList->PathStroke(color, false, thickness);
}
} // namespace ImGui
