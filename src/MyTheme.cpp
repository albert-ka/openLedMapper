#include "MyTheme.h"


ofColor getRandomColor()
{
    return ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255), 255);    
}

void MyTheme::setup()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    
    style->WindowMinSize = ImVec2(160, 65);
    style->FramePadding = ImVec2(4, 2);
    style->ItemSpacing = ImVec2(6, 2);
    style->ItemInnerSpacing = ImVec2(6, 4);
    style->Alpha = 1.0f;
    style->WindowRounding = 14.0f;
    style->FrameRounding = 12.0f;
    style->IndentSpacing = 6.0f;
    style->ItemInnerSpacing = ImVec2(2, 4);
    style->ColumnsMinSpacing = 50.0f;
    style->GrabMinSize = 14.0f;
    style->GrabRounding = 10.0f;
    style->ScrollbarSize = 12.0f;
    style->ScrollbarRounding = 10.0f;
    
    
    style->Colors[ImGuiCol_Text] = ImVec4(ofColor(255), 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(ofColor(128), 0.58f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(ofColor(15), 0.70f);
    style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(ofColor(255,255,255,0), 0.58f);
    style->Colors[ImGuiCol_Border] = ImVec4(ofColor(110,110,128,128), 0.00f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(ofColor(0), 0.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(ofColor(41,74,122,138), 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(ofColor(66,150,250,102), 0.78f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(ofColor(66,150,250,171), 1.00f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(ofColor(10,10,10,255), 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(ofColor(0,0,0,130), 0.75f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(ofColor(41,74,122,255), 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(ofColor(36,36,36,255), 0.47f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(ofColor(5,5,5,135), 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(ofColor(79,79,79,255), 0.21f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(ofColor(105,105,105,255), 0.78f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(ofColor(130,130,130,255), 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(ofColor(66,150,250,255), 0.80f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(ofColor(61,133,224,255), 0.14f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(ofColor(66,150,250,255), 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(ofColor(66,150,250,102), 0.14f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(ofColor(66,150,250,255), 0.86f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(ofColor(15,135,250,255), 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(ofColor(66,150,250,79), 0.76f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(ofColor(66,150,250,204), 0.86f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(ofColor(66,150,250,255), 1.00f);
    style->Colors[ImGuiCol_Column] = ImVec4(ofColor(66,150,250,102), 0.32f);
    style->Colors[ImGuiCol_ColumnHovered] = ImVec4(ofColor(66,150,250,255), 0.78f);
    style->Colors[ImGuiCol_ColumnActive] = ImVec4(ofColor(15,135,250,255), 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(ofColor(66,150,250,102), 0.04f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(ofColor(66,150,250,204), 0.78f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(ofColor(66,150,250,242), 1.00f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(ofColor(156,156,156,255), 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(ofColor(255,110,89,255), 1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(ofColor(230,179,0,255), 0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(ofColor(255,153,0,255), 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(ofColor(255,255,255,255), 0.43f); // ??
    style->Colors[ImGuiCol_PopupBg] = ImVec4(ofColor(255,255,255,255), 0.92f); // ??
    style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(ofColor(255,255,255,255), 0.73f); // ??
    
}
