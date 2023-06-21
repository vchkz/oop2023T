/**
* @file imgui_tabs.h
*
* @brief ImGui Tab Extensions
*
* This file contains extensions for ImGui that provide tab functionality.
*/

#ifndef IMGUI_TABS_H
#define IMGUI_TABS_H

#include <imgui.h>
#include <string>
#include <vector>

namespace ImGui {

/**
* @brief Display tab labels and manage tab selection.
*
* This function displays tab labels and allows the user to select a tab. It takes a vector of tab names, the size of the tab array, the current tab index, and an optional tab order array.
*
* @param tabNames The vector of tab names.
* @param tabSize The size of the tab array.
* @param tabIndex The current tab index. It will be updated with the selected tab index.
* @param tabOrder The optional tab order array.
*
* @return Returns true if the tab selection has changed, false otherwise.
*/
IMGUI_API bool TabLabels(const std::vector<std::string>& tabNames, size_t tabSize, size_t& tabIndex, int* tabOrder = NULL);

/**
* @brief Color picker widget.
*
* This function displays a color picker widget using ImGui. It takes a label for the color picker, and a float array of size 3 representing the RGB color value.
*
* @param label The label for the color picker.
* @param col The RGB color value.
*
* @return Returns true if the color value has changed, false otherwise.
*/
IMGUI_API bool ColorPicker(const char* label, float col[3]);

/**
* @brief Spinner widget.
*
* This function displays a spinner widget using ImGui. It takes a label for the spinner, the radius of the spinner, the thickness of the spinner, and the color of the spinner.
*
* @param label The label for the spinner.
* @param radius The radius of the spinner.
* @param thickness The thickness of the spinner.
* @param color The color of the spinner.
*
* @return Returns true if the spinner is active, false otherwise.
*/
IMGUI_API bool Spinner(const char* label, float radius, int thickness, const ImU32& color);

} // namespace ImGui

#endif // IMGUI_TABS_H
