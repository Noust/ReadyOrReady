#pragma once
#include "include.h"
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx9.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_internal.h>

void inputHandler();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#define clearVariable(x) if (x) { x->Release(); x = nullptr; delete x;}



void Colors() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(11, 12);
    style.FrameRounding = 4.0f;
    style.ItemSpacing = { 14.0f,4.0f };
    style.ScrollbarSize = 11.0f;
    style.WindowBorderSize = 0.0f;
    style.WindowRounding = 12.0f;
    style.ChildRounding = 12.0f;
    style.ScrollbarRounding = 12.0f;
    style.WindowTitleAlign = { 0.50f,0.50f };

    style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

}

std::string generateRandomString(int length) {
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string randomString;
    std::srand(std::time(0));
    for (int i = 0; i < length; ++i) {
        randomString += characters[std::rand() % characters.length()];
    }
    return randomString;
}

int generateRandomInt(int min, int max) {
    std::srand(std::time(0));
    return min + std::rand() % (max - min + 1);
}

DWORD getProcessID(std::string processName) {
    PROCESSENTRY32 processInfo;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    processInfo.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnapshot, &processInfo)) {
        do {
            if (!lstrcmpi(processInfo.szExeFile, processName.c_str())) {
                CloseHandle(hSnapshot);
                return processInfo.th32ProcessID;
            }
        } while (Process32Next(hSnapshot, &processInfo));
    }
    CloseHandle(hSnapshot);
    return 0;
}

typedef struct
{
    DWORD R;
    DWORD G;
    DWORD B;
    DWORD A;
}RGBA;

std::string stringToUTF8(const std::string& str) {
    int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
    wchar_t* pwBuf = new wchar_t[nwLen + 1];
    ZeroMemory(pwBuf, nwLen * 2 + 2);
    ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
    int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
    char* pBuf = new char[nLen + 1];
    ZeroMemory(pBuf, nLen + 1);
    ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
    std::string retStr(pBuf);
    delete[]pwBuf;
    delete[]pBuf;
    pwBuf = NULL;
    pBuf = NULL;
    return retStr;
}

void drawStrokeText(int x, int y, RGBA* color, const char* str) {
    ImFont a;
    std::string utf_8_1 = std::string(str);
    std::string utf_8_2 = stringToUTF8(utf_8_1);
    ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
    ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
    ImGui::GetForegroundDrawList()->AddText(ImVec2(x - 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
    ImGui::GetForegroundDrawList()->AddText(ImVec2(x + 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
    ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), utf_8_2.c_str());
}

void drawNewText(int x, int y, RGBA* color, const char* str) {
    ImFont a;
    std::string utf_8_1 = std::string(str);
    std::string utf_8_2 = stringToUTF8(utf_8_1);
    ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), utf_8_2.c_str());
}

void DrawLine(fvector2d pos1, fvector2d pos2, ImColor color, float Thicknes, bool checkpoints) {
    if (checkpoints) {
        if (pos1.x >= 0 && pos1.y >= 0 && pos1.x <= widthscreen && pos1.y <= heightscreen && pos2.x >= 0 && pos2.y >= 0 && pos2.x <= widthscreen && pos2.y <= heightscreen) {
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos1.x, pos1.y), ImVec2(pos2.x, pos2.y), ImColor(0, 0, 0), Thicknes + 2);
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos1.x, pos1.y), ImVec2(pos2.x, pos2.y), color, Thicknes);
        }
    }
    else {
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos1.x, pos1.y), ImVec2(pos2.x, pos2.y), ImColor(0, 0, 0), Thicknes + 2);
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos1.x, pos1.y), ImVec2(pos2.x, pos2.y), color, Thicknes);
    }
}

void DrawCircle(fvector2d pos, int radious, int thickness, ImColor color) {
    if (pos.x >= 0 && pos.y >= 0 && pos.x <= widthscreen && pos.y <= heightscreen) {
        ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(pos.x, pos.y), radious, ImColor(0, 0, 0), 0, thickness + 2);
        ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(pos.x, pos.y), radious, color, 0, thickness);
    }
}

void DrawCornerEsp(float W, float H, fvector2d pos, ImColor color, int thickness) {
    float lineW = (W / 5);
    float lineH = (H / 6);

    //outline
    DrawLine({ pos.x - W / 2, pos.y - H }, { pos.x - W / 2 + lineW, pos.y - H }, color, thickness, false);//top left
    DrawLine({ pos.x - W / 2, pos.y - H }, { pos.x - W / 2, pos.y - H + lineH }, color, thickness, false);
    DrawLine({ pos.x - W / 2, pos.y - lineH }, { pos.x - W / 2, pos.y }, color, thickness, false); //bot left
    DrawLine({ pos.x - W / 2, pos.y }, { pos.x - W / 2 + lineW, pos.y }, color, thickness, false);
    DrawLine({ pos.x + W / 2 - lineW, pos.y - H }, { pos.x + W / 2, pos.y - H }, color, thickness, false); // top right
    DrawLine({ pos.x + W / 2, pos.y - H }, { pos.x + W / 2, pos.y - H + lineH }, color, thickness, false);
    DrawLine({ pos.x + W / 2, pos.y - lineH }, { pos.x + W / 2, pos.y }, color, thickness, false); // bot right
    DrawLine({ pos.x + W / 2 - lineW, pos.y }, { pos.x + W / 2, pos.y }, color, thickness, false);
}

void DrawFilledRect(fvector2d pos, float height, float width, ImColor color) {
    ImGui::GetBackgroundDrawList()->AddRectFilled({ pos.x - width,pos.y - height }, { pos.x + width, pos.y }, color);
}

void drawbox(fvector2d pos, float height, float width, ImColor color, float thickness) {
    DrawLine({ pos.x + width, pos.y }, { pos.x - width,pos.y }, color, thickness, false);
    DrawLine({ pos.x + width, pos.y }, { pos.x + width,pos.y - height }, color, thickness, false);
    DrawLine({ pos.x + width,pos.y - height }, { pos.x - width,pos.y - height }, color, thickness, false);
    DrawLine({ pos.x - width,pos.y - height }, { pos.x - width,pos.y }, color, thickness, false);
}

void DrawString(fvector2d pos, ImColor color, float scale, const char* text) {
    // Get font size scaled by distance
    float fontSize = ImGui::GetFontSize() * scale;

    // Calculate text dimensions for centering
    ImVec2 textSize = ImGui::CalcTextSize(text);
    ImVec2 textPos = ImVec2(pos.x - (textSize.x * scale) / 2, pos.y);

    // Draw the text with scaling
    ImGui::GetBackgroundDrawList()->AddText(
        ImGui::GetFont(),
        fontSize,
        textPos,
        color,
        text
    );
}

void drawhealthbar(fvector2d pos, float height, float width, ImColor color, float thickness) {
    DrawLine({ pos.x - width,pos.y - height }, { pos.x - width,pos.y }, color, thickness, false);
}

void drawarmorbar(fvector2d pos, float height, float width, ImColor color, float thickness) {
    DrawLine({ pos.x + width,pos.y - height }, { pos.x + width,pos.y }, color, thickness, false);
}

ImColor HealthBarColor(ValidatedActorData actorData) {
    if (actorData.health <= actorData.maxHealth && actorData.health > actorData.maxHealth * 0.75)
        return ImColor(0, 255, 0);
    else if (actorData.health <= actorData.maxHealth * 0.75 && actorData.health > actorData.maxHealth * 0.5)
        return ImColor(85, 170, 0);
    else if (actorData.health <= actorData.maxHealth * 0.5 && actorData.health > actorData.maxHealth * 0.25)
        return ImColor(170, 85, 0);
    else if (actorData.health <= actorData.maxHealth * 0.25f && actorData.health > 0)
        return ImColor(255, 0, 0);
    return ImColor(0, 0, 0, 0);
}

void Draw3DBox(ValidatedActorData actorData, ImColor color, float Thickness, float width) {
    fvector pos1 = actorData.position;
    fvector pos2 = actorData.position;
    fvector pos3 = actorData.position;
    fvector pos4 = actorData.position;
    fvector pos5 = actorData.position;
    fvector pos6 = actorData.position;
    fvector pos7 = actorData.position;
    fvector pos8 = actorData.position;

    //Bottom Points
    pos1.x += width;
    pos1.y += width;
    pos1.z -= 90;
    pos2.x += width;
    pos2.y -= width;
    pos2.z -= 90;
    pos3.x -= width;
    pos3.y -= width;
    pos3.z -= 90;
    pos4.x -= width;
    pos4.y += width;
    pos4.z -= 90;

    //Top Points
    pos5.x += width;
    pos5.y += width;
    pos5.z += 80;
    pos6.x += width;
    pos6.y -= width;
    pos6.z += 80;
    pos7.x -= width;
    pos7.y -= width;
    pos7.z += 80;
    pos8.x -= width;
    pos8.y += width;
    pos8.z += 80;

    //to screen
    fvector2d posscreen1 = w2s(pos1);
    fvector2d posscreen2 = w2s(pos2);
    fvector2d posscreen3 = w2s(pos3);
    fvector2d posscreen4 = w2s(pos4);

    fvector2d posscreen5 = w2s(pos5);
    fvector2d posscreen6 = w2s(pos6);
    fvector2d posscreen7 = w2s(pos7);
    fvector2d posscreen8 = w2s(pos8);

    //Draw Top
    DrawLine(posscreen1, posscreen2, color, Thickness, true);
    DrawLine(posscreen2, posscreen3, color, Thickness, true);
    DrawLine(posscreen3, posscreen4, color, Thickness, true);
    DrawLine(posscreen4, posscreen1, color, Thickness, true);

    DrawLine(posscreen5, posscreen6, color, Thickness, true);
    DrawLine(posscreen6, posscreen7, color, Thickness, true);
    DrawLine(posscreen7, posscreen8, color, Thickness, true);
    DrawLine(posscreen8, posscreen5, color, Thickness, true);

    DrawLine(posscreen5, posscreen1, color, Thickness, true);
    DrawLine(posscreen6, posscreen2, color, Thickness, true);
    DrawLine(posscreen7, posscreen3, color, Thickness, true);
    DrawLine(posscreen8, posscreen4, color, Thickness, true);
}

void DrawBackgroundAnimation() {
    static std::vector<ImVec2> particles;
    static std::vector<float> particleAngles;
    static std::vector<float> particleSpeeds;
    static bool initialized = false;
    static float spawnTimer = 0.0f;
    const float SPAWN_INTERVAL = 0.5f;

    // Initialize particles if not done yet
    if (!initialized) {
        for (int i = 0; i < 100; i++) {
            particles.push_back(ImVec2(
                static_cast<float>(rand() % static_cast<int>(widthscreen)),
                static_cast<float>(rand() % static_cast<int>(heightscreen))
            ));
            particleAngles.push_back(static_cast<float>(rand()) / RAND_MAX * 2 * 3.14159f);
            particleSpeeds.push_back(0.2f + static_cast<float>(rand()) / RAND_MAX * 0.3f);
        }
        initialized = true;
    }

    // Spawn new particles over time
    spawnTimer += ImGui::GetIO().DeltaTime;
    if (spawnTimer >= SPAWN_INTERVAL && particles.size() < 150) {
        particles.push_back(ImVec2(
            static_cast<float>(rand() % static_cast<int>(widthscreen)),
            static_cast<float>(rand() % static_cast<int>(heightscreen))
        ));
        particleAngles.push_back(static_cast<float>(rand()) / RAND_MAX * 2 * 3.14159f);
        particleSpeeds.push_back(0.2f + static_cast<float>(rand()) / RAND_MAX * 0.3f);
        spawnTimer = 0.0f;
    }

    ImVec2 mousePos = ImGui::GetMousePos();
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
    float deltaTime = ImGui::GetIO().DeltaTime;

    // Update and draw each particle
    for (size_t i = 0; i < particles.size(); i++) {
        auto& particle = particles[i];
        auto& angle = particleAngles[i];
        auto& speed = particleSpeeds[i];

        // Floating motion
        particle.x += cos(angle) * speed;
        particle.y += sin(angle) * speed;

        // Slowly rotate angle for smooth wave-like motion
        angle += deltaTime * 0.3f;

        // Mouse interaction
        float dx = particle.x - mousePos.x;
        float dy = particle.y - mousePos.y;
        float dist = sqrt(dx * dx + dy * dy);

        const float MAX_INFLUENCE_DIST = 150.0f;
        const float MIN_INFLUENCE_DIST = 50.0f;

        if (dist < MAX_INFLUENCE_DIST) {
            float movement_factor;
            if (dist < MIN_INFLUENCE_DIST) {
                movement_factor = 1.0f;
            }
            else {
                movement_factor = 1.0f - ((dist - MIN_INFLUENCE_DIST) / (MAX_INFLUENCE_DIST - MIN_INFLUENCE_DIST));
            }

            particle.x += (dx / dist) * movement_factor * 3.0f;
            particle.y += (dy / dist) * movement_factor * 3.0f;
        }

        // Screen wrapping
        if (particle.x < 0) particle.x = widthscreen;
        if (particle.x > widthscreen) particle.x = 0;
        if (particle.y < 0) particle.y = heightscreen;
        if (particle.y > heightscreen) particle.y = 0;

        // Draw particle with pulsating opacity
        float opacity = 0.4f + 0.2f * sin(ImGui::GetTime() * speed * 2.0f);
        draw_list->AddCircleFilled(particle, 2.0f, ImColor(255, 255, 255, static_cast<int>(opacity * 255)));
    }
}

template <typename T>
void centertext(const char* text, T value1, T value2) {
    char texto[100];
    sprintf_s(texto, sizeof(texto), text, value1, value2);
    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(texto).x) / 2);
    ImGui::Text(texto, value1, value2);
}

struct animator {
    float alpha;
    bool tick;
    float speed;
};

void animatecontent(animator& animator) {
    const int limit = 255;
    if (animator.tick || animator.alpha == limit) {
        animator.tick = true;
        if (!(animator.alpha <= 0))
            animator.alpha -= animator.speed;
        else if (animator.alpha <= 0)
            animator.tick ^= 1;
    }
    else if (!animator.tick || animator.alpha != limit) {
        animator.tick = false;
        if (!(animator.alpha >= limit))
            animator.alpha += animator.speed;
        else if (animator.alpha >= limit)
            animator.tick ^= 1;
    }
}
