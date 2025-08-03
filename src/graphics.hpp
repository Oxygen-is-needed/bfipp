#ifndef GRAPHICS__HPP
#define GRAPHICS__HPP
#ifndef DISABLE_GRAPHICS

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

// Graphics {{{
namespace Graphics {
  GLFWwindow* window = nullptr;

  struct Log::O g_error = {
      .e = true,
      .lm = Log::GRAPHICS,
  };

  namespace End {
    void cleanup() {
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();

      glfwDestroyWindow(window);
      glfwTerminate();
    }

    void end() {
      Unwind::pop_unwind(cleanup);
      cleanup();
    }
    void start() {
      Unwind::add_unwind({
          .fptr = cleanup,
          .name = "Graphics::End::cleanup",
      });
    }
  }

  static void glfw_error_callback(int error, const char* description) {
    Log::print(g_error, "GLFW Error ", error, ": ", description);
  }

  bool centerButton(const char* label, float alignment = 0.5f) {
    ImGuiStyle& style = ImGui::GetStyle();

    float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
    float avail = ImGui::GetContentRegionAvail().x;

    float off = (avail - size) * alignment;
    if (off > 0.0f) {
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
    }

    return ImGui::Button(label);
  }

  bool is_setup = false;
  bool setup() {
    if (is_setup == true)
      return true;

    glfwSetErrorCallback(Graphics::glfw_error_callback);
    if (!glfwInit()) {
      Log::print(Graphics::g_error, "Failed to initalize glfw");
      return false;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    const char* glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    // Create window with graphics context
    float main_scale =
        ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
    Graphics::window =
        glfwCreateWindow((int)(1280 * main_scale), (int)(800 * main_scale),
                         "BFI++", nullptr, nullptr);
    if (Graphics::window == nullptr) {
      Log::print(Graphics::g_error, "glfw failed to create Graphics::window.");
      return false;
    }
    End::start();

    glfwMakeContextCurrent(Graphics::window);
    glfwSwapInterval(1);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // Disable Log and Ini Garbage
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(Graphics::window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    is_setup = true;
    return true;
  }

  struct Main_Function {
    bool e = false;
    size_t index = 0;
    void (*const fptr)(Main_Function&);
  };
  std::vector<Main_Function> main_functions;
  static bool kill_me = false;

  void mainFuncAdd(void (*const fptr)(Main_Function&), bool enable = true) {
    main_functions.push_back({
        .e = enable,
        .index = main_functions.size(),
        .fptr = fptr,
    });
  }

  bool main() {
    if (glfwWindowShouldClose(window)) {
      return false;
    }
    if (kill_me == true) {
      kill_me = false;
      return false;
    }

    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    glfwPollEvents();
    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      return true;
    }

    // Start Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Run added functions
    for (auto& f : main_functions) {
      if (f.e == true)
        f.fptr(f);
    }

    // Render
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(Graphics::window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(Graphics::window);

    return true;
  }
}
//}}}

#endif
#endif
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
