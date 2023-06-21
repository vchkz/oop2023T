#include "program.h"
#include <glad/glad.h>

#include <atomic>

#include <imgui.h>
#include <imgui_internal.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <nfd.h>

#include "include/IconsFontAwesome4.h"

#include "glarraybuffer.h"
#include "glprogram.h"
#include "images.h"
#include "fstream"
#include "imgui_extension.h"


#include "yolo_seg.h"
#include "yolov5_seg_utils.h"


#include<opencv2//opencv.hpp>


#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/string_cast.hpp>
#include <thread>
#include "image.h"

#include <filesystem>


Image image;
GlProgram imageShader;
GlProgram backgroundShader;
GlArrayBuffer buffer;

std::atomic<bool> showPopup(false);

static struct {
    bool show_toolbar = false;
    bool show_tooloptions = false;
    bool show_content = false;
    bool show_dockbar = false;
    int width = 200;
    int height = 300;
    int mousex = 0;
    int mousey = 0;
    int mouseImagex = 0;
    int mouseImagey = 0;
    int zoom = 100;
    int translatex = 0;
    int translatey = 0;
    bool shiftPressed = false;
    bool ctrlPressed = false;
    bool spacePressed = false;
    glm::vec2 contentPosition;
    glm::vec2 contentSize;
    bool mousePanning = false;
    bool isAutoSavingEnable = false;
    bool maskIsCreated = false;
    bool autoMaskCreating = false;

} state;

Program::Program(GLFWwindow *window)
        : _window(window) {
    glfwSetWindowUserPointer(this->_window, static_cast<void *>(this));
}

Program::~Program() {
    glfwSetWindowUserPointer(this->_window, nullptr);
}

#define GLSL(src) "#version 150\n" #src

std::string vertexGlsl = GLSL(
                                 in vec3 vertex;
                                 in vec2 texcoord;

                                 uniform mat4 u_projection;
                                 uniform mat4 u_view;

                                 out vec2 f_texcoord;

                                 void main() {
                                     gl_Position = u_projection * u_view * vec4(vertex.xyz, 1.0);
                                     f_texcoord = texcoord;
                                 });

std::string fragmentGlsl = GLSL(
                                   uniform sampler2D u_texture;

                                   in vec2 f_texcoord;

                                   out vec4 color;

                                   void main() {
                                       color = texture(u_texture, f_texcoord);
                                   });

std::string vertexBlocksGlsl = GLSL(
                                       in vec3 vertex;
                                       in vec2 texcoord;

                                       uniform mat4 u_projection;
                                       uniform mat4 u_view;

                                       out vec2 f_texcoord;

                                       void main() {
                                           gl_Position = u_projection * u_view * vec4(vertex.xyz, 1.0);
                                           f_texcoord = texcoord;
                                       });

std::string fragmentBlocksGlsl = GLSL(
                                         in vec2 f_texcoord;
                                         out vec4 color;

                                         void main() {
                                             if (int(gl_FragCoord.x) % 32 < 16 && int(gl_FragCoord.y) % 32 > 16 ||
                                                                       int(gl_FragCoord.x) % 32 > 16 &&
                                                                       int(gl_FragCoord.y) % 32 < 16)
                                             color = vec4(0.9f, 0.9f, 0.92f, 1.0f);
                                             else
                                             color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
                                         });


static size_t selectedTab = 0;
std::vector<std::string> tabNames;
static size_t tabNameAllocCount = 0;
const int dockbarWidth = 250;
const int menubarHeight = 22;
const int optionsbarHeight = 45;
const int statebarHeight = 35;

std::vector<std::string> filenames;
std::filesystem::path savingDirectory;


void openSavingDirectory() {
    nfdchar_t *outPath = nullptr;
    nfdresult_t result = NFD_PickFolder(nullptr, &outPath);
    if (result == NFD_OKAY) {
        savingDirectory = std::filesystem::path(outPath);
    }

}


void openImage() {
    nfdchar_t *outPath = nullptr;
    nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, &outPath);

    if (result == NFD_OKAY) {
        image.fullPath = outPath;
        image.name = image.fullPath.substr(image.fullPath.find_last_of('\\') + 1);

        image.fromFile(outPath);


        std::filesystem::path parentPath = std::filesystem::path(image.fullPath).parent_path();


        for (const auto &entry: std::filesystem::directory_iterator(parentPath)) {
            if (entry.is_regular_file()) {
                filenames.push_back(entry.path().string());
            }
        }

        std::sort(filenames.begin(), filenames.end(), std::less<std::string>());

        int fileNumber = 0;
        for (const auto &filename: filenames) {
            if (filename == image.fullPath) {
                image.indexInDirectory = fileNumber;
                break;
            }
            fileNumber++;
        }

        if (savingDirectory.empty()) savingDirectory = parentPath;

    }
}

void saveMask(std::string savePath) {
    auto maskLayer = image.getMask();
    int width = maskLayer->size[0];
    int height = maskLayer->size[1];

    cv::Mat resultMask(height, width, CV_8UC4);
    std::memcpy(resultMask.data, maskLayer->data, width * height * 4 * sizeof(unsigned char));
    cv::cvtColor(resultMask, resultMask, cv::COLOR_BGR2GRAY);
    for (int i = 0; i < resultMask.rows; ++i) {
        for (int j = 0; j < resultMask.cols; ++j) {
            uchar pixelValue = resultMask.at<uchar>(i, j);
            if (pixelValue == 0) {
                resultMask.at<uchar>(i, j) = 255;
            } else {
                resultMask.at<uchar>(i, j) = 0;
            }
        }
    }

    cv::imwrite(savePath, resultMask);

}


void singleSave() {

    nfdchar_t *savePath = NULL;
    nfdresult_t result = NFD_SaveDialog("jpg", image.name.c_str(), &savePath);
    if (result == NFD_OKAY) {
        saveMask(std::string(savePath) + ".jpg");
    }
}

void autoSave() {
    if (state.isAutoSavingEnable) {
        std::filesystem::path filepath = savingDirectory / ("mask_" + image.name);
        image.setDirtyForSaving(false);
        saveMask(filepath.string());
    }
}

void newMask() {
    std::filesystem::path currentDir = std::filesystem::current_path();

    std::string model_name;


    if (std::filesystem::exists(currentDir/"include")){currentDir/="include";}
    if (std::filesystem::exists(currentDir/"../include")){currentDir = currentDir.parent_path()/"include";}

    for (const auto &entry: std::filesystem::directory_iterator(currentDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".onnx") {
            model_name = entry.path().string();
            break;
        }
    }

    if (model_name.empty()) {
        throw std::runtime_error("File .onnx not found.");
    }



    YoloSeg test;
    cv::dnn::Net net;
    if (test.ReadModel(net, model_name, true)) {
    } else {
        return;
    }

    std::vector<OutputSeg> result;

    std::string img_path = image.fullPath;
    cv::Mat img = cv::imread(img_path);


    if (!test.Detect(img, net, result)) { throw std::runtime_error("Detect Failed!"); }


    showPopup = false;
    cv::Mat mask = img.clone();
    mask.setTo(0);
    for (int i = 0; i < result.size(); i++) {
        mask(result[i].box).setTo(255, result[i].boxMask);
    }
    image.fromMAT(mask, std::string("mask"));

    return;

}


void CtrlZ() {
    image.Undo();
}

void startScreen() {}

void nextImage() {
    if (image.indexInDirectory < filenames.size()) {
        image.indexInDirectory++;
        image.fullPath = filenames[image.indexInDirectory];
        image.name = image.fullPath.substr(image.fullPath.find_last_of('\\') + 1);
        image.fromFile(image.fullPath.c_str());
        image.maskIsCreated = false;
        image.setDirty(true);

    }


}

void previousImage() {
    if (image.indexInDirectory > 0) {
        image.indexInDirectory--;
        image.fullPath = filenames[image.indexInDirectory];
        image.name = image.fullPath.substr(image.fullPath.find_last_of('\\') + 1);
        image.fromFile(image.fullPath.c_str());
        image.maskIsCreated = false;
        image.setDirty(true);

    }
}


bool Program::SetUp() {
    ImGuiIO &io = ImGui::GetIO();

    ImFontConfig config;
    config.MergeMode = true;
    config.PixelSnapH = true;


    const char *fontPath = "include/Roboto-Medium.ttf";
    if (std::ifstream(fontPath).good()) {
        io.Fonts->AddFontFromFileTTF(fontPath, 16.0f);
        ImGui::GetIO().FontDefault = io.Fonts->Fonts.back();

        static const ImWchar icons_ranges_fontawesome[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
        io.Fonts->AddFontFromFileTTF("include/fontawesome-webfont.ttf", 16.0f, &config, icons_ranges_fontawesome);

    } else {
        fontPath = "../include/Roboto-Medium.ttf";
        if (std::ifstream(fontPath).good()) {
            io.Fonts->AddFontFromFileTTF(fontPath, 16.0f);
            ImGui::GetIO().FontDefault = io.Fonts->Fonts.back();

            static const ImWchar icons_ranges_fontawesome[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
            io.Fonts->AddFontFromFileTTF("../include/fontawesome-webfont.ttf", 16.0f, &config, icons_ranges_fontawesome);
        } else {
            io.Fonts->AddFontDefault();
        }
    }

    imageShader.init(vertexGlsl.c_str(), fragmentGlsl.c_str());
    backgroundShader.init(vertexBlocksGlsl.c_str(), fragmentBlocksGlsl.c_str());
    buffer.init();

    startScreen();


    return true;
}

void Program::Render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    glViewport(0, 0, state.width, state.height);
    glClearColor(50 / 255.0f, 50 / 255.0f, 50 / 255.0f, 50 / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (image.isOpen) {

        if (image.isDirty()) image.texImage();

        if (image.isDirtyForSaving()) autoSave();

        auto zoom = glm::scale(glm::mat4(1.0f), glm::vec3(state.zoom / 100.0f));
        auto translate = glm::translate(zoom, glm::vec3(state.translatex, state.translatey, 0.0f));
        auto scale = glm::scale(translate, glm::vec3(image.size[0], image.size[1], 1.0f));

        auto projection = glm::ortho(-(state.width / 2.0f), (state.width / 2.0f), (state.height / 2.0f),
                                     -(state.height / 2.0f));

        buffer.bind();

        backgroundShader.bind()
                .matrix("u_projection", projection)
                .matrix("u_view", scale);
        buffer.render();

        imageShader.bind()
                .matrix("u_projection", projection)
                .matrix("u_view", scale);
        buffer.render();

        buffer.unbind();
    }


    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 1.0f);
    {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4());
        {
            if (ImGui::BeginMainMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    if (ImGui::MenuItem("Open Image", "CTRL+O")) openImage();
                    ImGui::Separator();
                    if (ImGui::MenuItem("Save Mask", "CTRL+S", false, image.maskIsCreated))
                        singleSave();
                    if (ImGui::MenuItem("Change saving directory", nullptr)) openSavingDirectory();
                    if (ImGui::MenuItem("Auto saving", nullptr, state.isAutoSavingEnable))
                        state.isAutoSavingEnable = !state.isAutoSavingEnable;
                    ImGui::Separator();
                    if (ImGui::MenuItem("Quit")) {
                        glfwSetWindowShouldClose(_window, GLFW_TRUE);
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Edit")) {
                    if (ImGui::MenuItem("Undo", "CTRL+Z", false, image.UndoStackSize() > 0))
                        CtrlZ();
                    ImGui::Separator();
                    if (ImGui::MenuItem("Auto mask creating", nullptr, state.autoMaskCreating, true))
                        state.autoMaskCreating = !state.autoMaskCreating;

                    ImGui::EndMenu();
                }


                ImGui::EndMainMenuBar();

            }


        }
        ImGui::PopStyleColor();


        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.20f, 0.20f, 0.20f, 0.60f));
        {


            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4());
            {
                ImGui::Begin("content", &(state.show_content),
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
                {
                    ImGui::SetWindowPos(ImVec2(state.contentPosition.x, state.contentPosition.y));
                    ImGui::SetWindowSize(ImVec2(state.contentSize.x, state.contentSize.y));

                }
                ImGui::End();
            }
            ImGui::PopStyleColor();


            ImGui::Begin("dockbar", &(state.show_dockbar),
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
            {
                ImGui::SetWindowPos(ImVec2(float(state.width - dockbarWidth), float(menubarHeight)));

                ImGui::SetWindowSize(ImVec2(float(dockbarWidth),
                                            float(state.height - menubarHeight - optionsbarHeight - statebarHeight)));
                ImGui::Text("%s", image.name.c_str());

                static bool colorOptionsOpen = true;
                if (ImGui::CollapsingHeader("Color options", &colorOptionsOpen, ImGuiTreeNodeFlags_DefaultOpen)) {

                    ImGui::ColorPicker("rgb", image.brushColor);

                }

                if (image.isOpen) {
                    static bool layerOptionsOpen = true;
                    if (ImGui::CollapsingHeader("Layer options", &layerOptionsOpen, ImGuiTreeNodeFlags_DefaultOpen)) {

                        if (image.maskIsCreated) {
                            auto mask = image.getMask();

                            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.7f, 0.7f, 0.5f));
                            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 0.5f));
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 0.5f));

                            if (ImGui::Button("mask", ImVec2(180, 30))) {
                                mask->toggleVisibility();
                                image.setDirty(true);
                            }
                            ImGui::PopStyleColor(2);
                            ImGui::SameLine();

                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 0.5f));

                            if (ImGui::Button(mask->isVisible() ? ICON_FA_EYE : ICON_FA_EYE_SLASH, ImVec2(-1, 30))) {
                                mask->toggleVisibility();
                                image.setDirty(true);
                            }
                            if (ImGui::SliderFloat("Alpha", &(image.alpha), 0.0f, 1.0f)) {
                                image.setDirty(true);
                            }
                            ImGui::PopStyleColor(2);

//

                        } else {
                            static std::thread popupThread;


                            if (!showPopup && popupThread.joinable()) {
                                popupThread.join();
                                image.maskIsCreated = true;
                                image.setDirtyForSaving(true);
                                image.setDirty(true);
                            }

                            if ((!showPopup && state.autoMaskCreating && !image.maskIsCreated) ||
                                (!showPopup && ImGui::Button((std::string(ICON_FA_PLUS) + " Create mask").c_str(),
                                                             ImVec2(-1.0f, 30.0f)))) {
                                showPopup = true;
                                popupThread = std::thread(newMask);
                            }

                            if (showPopup) {
                                float offset =
                                        (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x -
                                         30) * 0.5f;
                                ImGui::SetCursorPosX(offset);
                                const ImU32 col = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
                                ImGui::Spinner("##spinner", 15, 6, col);
                            }
                        }
                        ImGui::Separator();
                    }
                }


            }
            ImGui::End();

        }
        ImGui::PopStyleColor();
    }
    ImGui::PopStyleVar();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Program::onKeyAction(
        int key,
        int scancode,
        int action,
        int mods) {
    (void) key;
    (void) scancode;
    (void) action;

    state.shiftPressed = (mods & GLFW_MOD_SHIFT);
    state.ctrlPressed = (mods & GLFW_MOD_CONTROL);


    if (key == GLFW_KEY_SPACE) {
        if (action == GLFW_PRESS) {
            state.spacePressed = true;
        } else if (action == GLFW_RELEASE) {
            state.spacePressed = false;
        }

        return;
    }
    if (key == GLFW_KEY_O && action == GLFW_PRESS && (mods & GLFW_MOD_CONTROL)) openImage();
    if (key == GLFW_KEY_S && action == GLFW_PRESS && (mods & GLFW_MOD_CONTROL)) singleSave();
    if (key == GLFW_KEY_Z && action == GLFW_PRESS && (mods & GLFW_MOD_CONTROL)) CtrlZ();
    if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS) { nextImage(); }
    if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS) { previousImage(); }

}

bool isMouseInContent() {
    if (state.mousex < state.contentPosition.x) return false;
    if (state.mousey < state.contentPosition.y) return false;
    if (state.mousex > (state.contentPosition.x + state.contentSize.x)) return false;
    if (state.mousey > (state.contentPosition.y + state.contentSize.y)) return false;

    return true;
}

void Program::onMouseMove(
        int x,
        int y) {
    if (state.mousePanning || state.spacePressed) {
        state.translatex += ((x - state.mousex) * (100.0f / state.zoom));
        state.translatey += ((y - state.mousey) * (100.0f / state.zoom));

        state.mousex = x;
        state.mousey = y;

        return;
    }

    state.mousex = x;
    state.mousey = y;


    if (!isMouseInContent() && image.isOpen) {
        image.deleteCursor();
        return;
    }

    if (!isMouseInContent()) { return; }

    if (image.isOpen) {
        auto zoom = glm::scale(glm::mat4(1.0f), glm::vec3(state.zoom / 100.0f));
        auto translate = glm::translate(zoom, glm::vec3(state.translatex, -state.translatey, 0.0f));
        auto projection = glm::ortho(-(state.width / 2.0f), (state.width / 2.0f), (state.height / 2.0f),
                                     -(state.height / 2.0f));

        auto pp = glm::unProject(
                glm::vec3(x, y, 0.0f),
                translate, projection,
                glm::vec4(0.0f, 0.0f, state.width, state.height));

        state.mouseImagex = pp.x + (image.size[0] / 2.0f);
        state.mouseImagey = -(pp.y - (image.size[1] / 2.0f));

        image.mouseMove(state.mouseImagex, state.mouseImagey);


    }
}

void Program::onMouseButton(
        int button,
        int action,
        int mods) {
    state.mousePanning = false;
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        state.mousePanning = true;
        return;
    }

    if (!image.isOpen) return;


    if (!isMouseInContent()) return;


    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

        image.leftMouseButton();

    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) { image.rightMouseButton(); }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) { image.mouseButtonUp(); }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) { image.mouseButtonUp(); }

}

void Program::onScroll(
        int x,
        int y) {
    (void) x;


    if (state.ctrlPressed) {
        image.resizeCursor(y);
        image.drawCursor(state.mouseImagex, state.mouseImagey);
    } else {
        auto oldZoom = state.zoom;

        state.zoom += (y * 10);
        if (state.zoom < 10) state.zoom = 10;

        auto centerX = state.width / 2;
        auto centerY = state.height / 2;
        state.translatex -= ((centerX - state.mousex) * ((oldZoom - state.zoom) / 200.0f));
        state.translatey -= ((centerY - state.mousey) * ((oldZoom - state.zoom) / 200.0f));
    }
}

void Program::onResize(
        int width,
        int height) {
    state.width = width;
    state.height = height;
    state.contentPosition = glm::vec2(10, menubarHeight);
    state.contentSize = glm::vec2(state.width - 10 - dockbarWidth, state.height - menubarHeight);

    glViewport(0, 0, width, height);
}

void Program::CleanUp() {}

void Program::KeyActionCallback(
        GLFWwindow *window,
        int key,
        int scancode,
        int action,
        int mods) {
    auto app = static_cast<Program *>(glfwGetWindowUserPointer(window));

    if (app != nullptr) app->onKeyAction(key, scancode, action, mods);
}

void Program::CursorPosCallback(
        GLFWwindow *window,
        double x,
        double y) {
    auto app = static_cast<Program *>(glfwGetWindowUserPointer(window));

    if (app != nullptr) app->onMouseMove(int(x), int(y));
}

void Program::ScrollCallback(
        GLFWwindow *window,
        double x,
        double y) {
    auto app = static_cast<Program *>(glfwGetWindowUserPointer(window));

    if (app != nullptr) app->onScroll(int(x), int(y));
}

void Program::MouseButtonCallback(
        GLFWwindow *window,
        int button,
        int action,
        int mods) {
    auto app = static_cast<Program *>(glfwGetWindowUserPointer(window));

    if (app != nullptr) app->onMouseButton(button, action, mods);
}

void Program::ResizeCallback(
        GLFWwindow *window,
        int width,
        int height) {
    auto app = static_cast<Program *>(glfwGetWindowUserPointer(window));

    if (app != nullptr) app->onResize(width, height);
}
