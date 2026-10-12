/*
 * core.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

#include <algorithm>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include "gui/core.h"
#include "io/log.h"

namespace
{
    constexpr float overlay_margin = 10.0f;
    constexpr ImU32 warn_colour = IM_COL32(255, 204, 51, 255);
    constexpr ImU32 error_colour = IM_COL32(255, 77, 77, 255);
    constexpr ImU32 shadow_colour = IM_COL32(0, 0, 0, 200);

    // Drawn on the foreground draw list so it always sits in front of every window.
    void render_log_overlay() noexcept
    {
        const auto lines = st::io::log::overlay_lines();
        const bool any_visible =
            std::any_of(lines.begin(), lines.end(), [](const auto &line) { return !line.text.empty(); });
        if (!any_visible)
            return;

        ImGuiViewport *viewport = ImGui::GetMainViewport();
        const float    wrap_width = viewport->WorkSize.x - (2.0f * overlay_margin);
        if (wrap_width <= 0.0f)
            return;

        ImDrawList *draw_list = ImGui::GetForegroundDrawList();
        ImFont     *font = ImGui::GetFont();
        const float font_size = ImGui::GetFontSize();
        const float bottom = viewport->WorkPos.y + viewport->WorkSize.y;
        ImVec2      pos{viewport->WorkPos.x + overlay_margin, viewport->WorkPos.y + overlay_margin};
        for (const auto &line : lines)
        {
            if (pos.y >= bottom)
                break;
            const char *text = line.text.empty() ? " " : line.text.c_str();
            if (!line.text.empty())
            {
                draw_list->AddText(font, font_size, {pos.x + 1.0f, pos.y + 1.0f}, shadow_colour, text, nullptr,
                                   wrap_width);
                draw_list->AddText(font, font_size, pos, line.is_error ? error_colour : warn_colour, text, nullptr,
                                   wrap_width);
            }
            pos.y += ImGui::CalcTextSize(text, nullptr, false, wrap_width).y;
        }
    }
} // namespace

struct st::gui::core::core_impl
{
    core_impl() noexcept : m_show_demo(true) {}
    ~core_impl() noexcept = default;

    bool m_show_demo;
};

st::gui::core::core(const st::video::window &window) noexcept : m_impl{std::make_unique<core_impl>()}
{
    const char *glsl_version = nullptr;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle &style = ImGui::GetStyle();
    float       main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a solution for dynamic style scaling,
                                     // changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale; // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true
                                     // automatically overrides this for every window depending on the current monitor)

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(static_cast<SDL_Window *>(window.sdl_ptr()),
                                 static_cast<SDL_GLContext>(window.gl_ptr()));
    ImGui_ImplOpenGL3_Init(glsl_version);
}

st::gui::core::~core() noexcept
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void
st::gui::core::clear() noexcept
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void
st::gui::core::render() noexcept
{
#ifndef NDEBUG
    if (m_impl->m_show_demo)
        ImGui::ShowDemoWindow(&m_impl->m_show_demo);
#endif
    render_log_overlay();
    ImGui::Render();
}

void
st::gui::core::swap() noexcept
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void
st::gui::core::process_event(const SDL_Event &e) noexcept
{
    ImGui_ImplSDL3_ProcessEvent(&e);
}
