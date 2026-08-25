/*
 * gui_imgui.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 20/08/2026
 */

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

#include "st_includes.h"
#include "st_primitives.h"
#include "gui/backend/gui_backend.h"

#ifndef NDEBUG
static st_bool imgui_show_demo = true;
#endif

st_bool
st_gui_backend_init(void)
{
    IMGUI_CHECKVERSION();
    if (ImGui::CreateContext() == nullptr)
        return false;
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    st_float32 main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;

    return true;
}

st_bool
st_gui_backend_free(void)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    return true;
}

st_bool
st_gui_backend_create(SDL_Window *win,
                      SDL_GLContext gl_context)
{
    if (!ImGui_ImplSDL3_InitForOpenGL(win, gl_context))
        return false;
    if (!ImGui_ImplOpenGL3_Init())
        return false;
    return true;
}

st_bool
st_gui_backend_destroy(void)
{
    return true;
}

void
st_gui_backend_new_frame(void)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void
st_gui_backend_render_frame(void)
{
#ifndef NDEBUG
    if (imgui_show_demo)
        ImGui::ShowDemoWindow(&imgui_show_demo);
#endif
    ImGui::Render();
}

void
st_gui_backend_swap_frame(void)
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void
st_gui_backend_process_event(SDL_Event e)
{
    ImGui_ImplSDL3_ProcessEvent(&e);
}
