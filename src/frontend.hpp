#ifndef FRONTEND__HPP
#define FRONTEND__HPP

// Frontend Utilities {{{
namespace Frontend_Utils {
  void print_output(std::string output) {
    if (output.empty())
      return;

    std::cout << C::M << "\nOUTPUT =====\n"
              << C::RESET << output << C::M << "\n============" << C::RESET
              << std::endl;
    fflush(stdout);
  }

  std::string ic_default[3] = {
      "\033[" STR(M_BLUE) "m",    ///< First Parenthesis
      "\033[" STR(M_YELLOW) "m",  ///< Parenthesis content
      "\033[" STR(M_RESET) "m",   ///< End Parenthesis
  };

  std::string ic_none[3] = {"", "", ""};

  unsigned int inspect_buffer(VM& vm, ssize_t mwidth = 100, int view_frame = 14,
                              std::ostream& os = std::cout,
                              std::string (&ic)[3] = ic_default) {

    if (mwidth == -1) {
      mwidth = std::numeric_limits<int>::max();
    }

    int width = view_frame;
    unsigned int out = 5;
    os << vm.total_steps << ": ";
    unsigned int y = 0;
    for (int x = 0; x < width && y < mwidth && y < vm.rules.tape_length;
         x++, y++) {
      if (vm.buffer[y] != 0) {
        x = 0;
      }

      std::string code = "";
      if (vm.buffer[y] != 0) {
        if (isprint(vm.buffer[y]) == 0 || vm.buffer[y] == ' ') {
          code = std::to_string(vm.buffer[y]);
        } else {
          code += vm.buffer[y];
        }
      }

      if (vm.pc == y) {
        os << ic[0] << "{" << ic[1] << code << ic[0] << "} " << ic[2];
        continue;
      }

      os << "[" << code << "] ";
    }
    os << std::endl;
    out += 4 * y;
    return out;
  }

  void inspect_instructions(VM& vm, std::ostream& os = std::cout,
                            std::string (&ic)[3] = ic_default) {
    const unsigned int sub = 4;
    unsigned int cap = vm.ins_max;
    unsigned int frame_len = 9;
    unsigned int len = cap;
    unsigned int pc = vm.ins_i;
    unsigned int start;

    if (len < frame_len)
      frame_len = len;

    if (pc <= sub)
      start = 0;
    else
      start = pc - sub;

    unsigned int end = start + frame_len;
    if (end > cap)
      end = cap;

    os << "I: ";
    for (unsigned int x = start; x < end; x++) {
      if (x == pc) {
        os << ic[0] << "(" << ic[1] << vm.code[x] << ic[0] << ")" << ic[2];
        continue;
      }
      os << ic[0] << "[" << ic[2] << vm.code[x] << ic[0] << "]" << ic[2];
    }
    os << "\n   ";
    for (unsigned int x = start; x < end; x++) {
      os << "`" << x % 10 << "`";
    }
    os << " +" << (pc / 10) * 10 << std::endl;
  }
}
// }}}

// None {{{
namespace None {
  void frontend(VM& vm) {
    vm.run();

    Frontend_Utils::print_output(vm.output);
  }
}
// }}}
// SimpleTextFrontend {{{
namespace SimpleTextFrontend {
#ifndef DISABLE_FRONTEND__SIMPLE_TEXT
#define KEYS FRONTEND_KEYS__SIMPLE_TEXT

  enum Keybinds {
#define X(A, B, C) A = B,
    KEYS
#undef X
  };

  void help(std::ostream& s = std::cout) {
    s << "\n" FRONTEND_DESCRIPTION__SIMPLE_TEXT "\n";
#define X(A, B, C) Utils::print_help(B, "", C, false, s);
    KEYS
#undef X
  }

  unsigned int get_int() {
    std::string input;
    std::cin >> input;
    return static_cast<unsigned int>(std::stoul(input));
  }
  void keybindings(VM& vm, char& key, bool& ret, bool& ug, int& phelp,
                   unsigned int& skip, unsigned int& wins, char& wi,
                   unsigned int& wpc) {
    switch (key) {
      case QUIT:
        ret = false;
        return;
      case HELP:
        phelp = 2;
        return;

      case RUN:
        ug = false;
        return;
      case SKIP:
        skip = get_int();
        return;
      case WAIT:
        wins = get_int();
        return;
      case INSTRUCTION:
        wi = getchar();
        return;
      case CLEAR:
        vm.output.clear();
        return;
      case ADVANCE:
        wpc = get_int();
        return;
    }
  }

  std::string out = "";

  void unwind() {
    Frontend_Utils::print_output(out);
    C::show_cursor();
  }

  void frontend(VM& vm) {

    Frontend_Utils::inspect_buffer(vm);
    Frontend_Utils::inspect_instructions(vm);

    bool ret = true;
    bool user_guided = true;
    char wait_instruction = 0;
    int phelp = false;
    unsigned int skip = 0;
    unsigned int skip_i = 0;
    unsigned int wins = 0;
    unsigned int wpc = 0;

    C::hide_cursor();
    Unwind::add_unwind({unwind, "SimpleTextFrontend::unwind"});
    while (ret != false) {
      std::println("{} {}", wpc, vm.total_steps);
      // TODO: clean up if condition
      if (user_guided == true && skip_i++ >= skip &&
          (wins == 0 || wins < vm.ins_i) &&
          (wpc == 0 || wpc <= vm.total_steps) &&
          (wait_instruction == 0 ||
           (vm.ins_i < vm.ins_max && wait_instruction == vm.code[vm.ins_i]))) {
        skip_i = 0;
        wait_instruction = 0;
        char in = getchar();
        std::string input;

        keybindings(vm, in, ret, user_guided, phelp, skip, wins,
                    wait_instruction, wpc);
        if (ret == false)
          continue;

        C::clear();

        Frontend_Utils::inspect_buffer(vm, -1);
        Frontend_Utils::inspect_instructions(vm);
        std::cout << "\nOutput: " << vm.output;

        if (phelp > 0) {
          help();
          phelp--;
        }

        std::cout << std::endl;
      }
      ret = vm.step();
    }
    C::show_cursor();

    Frontend_Utils::print_output(vm.output);
    Unwind::pop_unwind();
  }

#else
  void (*const frontend)(VM& vm) = nullptr;
  void (*const help)() = nullptr;
#endif
#undef KEYS
}
// }}}
// SimpleGraphicalFrontend {{{
namespace SimpleGraphicalFrontend {
  namespace {  // <anonymous>
    VM* local_vm = nullptr;
    bool done = false;
  }

  struct TextFilters {
    static int filterNumerical(ImGuiInputTextCallbackData* data) {
      if (data->EventChar >= '0' && data->EventChar <= '9') {
        return 0;
      }
      return 1;
    }
  };

  static void menu() {
    if (!ImGui::BeginMenuBar())
      return;

    if (ImGui::BeginMenu("Main")) {
      if (ImGui::MenuItem("quit"))
        Graphics::kill_me = true;
      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }

  static void glSteps(const int& mult) {
    for (int x = 0; x < mult; x++) {
      if (local_vm->step() != false)
        continue;
      done = true;
      break;
    }
  }

  static void output(bool& t) {
    ImGui::BeginChild("Output", ImVec2(0, 0), ImGuiChildFlags_NavFlattened,
                      ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::TextWrapped("%s", local_vm->output.c_str());
    if (t == true)
      ImGui::SetScrollHereY(1.0f);
    ImGui::EndChild();
  }
  static void output(bool&& t = true) {
    output(t);
  }

  // TODO: split into multiple static functions
  void graphical_loop(Graphics::Main_Function&) {
    ImGui::SetNextWindowSize(ImVec2(500, 750), ImGuiCond_FirstUseEver);
    ImGui::Begin("Simple Graphical Frontend", nullptr,
                 ImGuiWindowFlags_MenuBar);

    menu();

    ImGui::Dummy(ImVec2(200.0f, 20.0f));

    static std::string buf = "";
    ImGui::TextUnformatted(buf.c_str());

    if (done == true) {
      output(false);
      ImGui::End();
      return;
    }

    std::stringstream ss;
    Frontend_Utils::inspect_buffer(*local_vm, 14, 14, ss,
                                   Frontend_Utils::ic_none);
    buf = ss.str();

    std::stringstream ssi;
    Frontend_Utils::inspect_instructions((*local_vm), ssi,
                                         Frontend_Utils::ic_none);
    ImGui::TextUnformatted(ssi.str().c_str());
    ImGui::Dummy(ImVec2(0.0f, 15.0f));

    static int step_mult = 1;
    if (ImGui::Button("Next")) {
      glSteps(step_mult);
    }
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
      ImGui::SetTooltip("Increment.");
    }

    {
      const static float rate = 60.0;

      static bool is_auto = false;
      ImGui::Checkbox("##", &is_auto);
      if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
        ImGui::SetTooltip("Enable Auto-Stepping.");
      }
      ImGui::SameLine();

      static float step_rate = 0.5f;
      static unsigned int step_time = 0;
      static unsigned int step_time_goal = rate * step_rate;
      if (ImGui::SliderFloat(
              "Increment", &step_rate, 0.0f, 5.0f, "%.2f",
              ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_AlwaysClamp)) {
        step_time_goal = static_cast<unsigned int>(rate * step_rate);
      }
      if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
        ImGui::SetTooltip("Amount of step progressions per second.");
      }

      ImGui::SliderInt("Step Size", &step_mult, 1, 10000, "%d",
                       ImGuiSliderFlags_Logarithmic);
      if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
        ImGui::SetTooltip("Amount of steps taken per increment.");
      }

      if (is_auto == true) {
        step_time++;
        static unsigned int num = 0;
        if (step_time >= step_time_goal) {
          num++;
          glSteps(step_mult);
          step_time = 0;
        }
      }
    }

    {
      if (ImGui::Button("Run")) {
        while (local_vm->step() != false) {}
        done = true;
      }

      if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
        ImGui::SetTooltip("Run entire program.");
      }
    }
    {
      static unsigned int error = 0;
      static std::string goto_text;
      ImGui::InputText("GOTO:", &goto_text);
      if (ImGui::Button("Instruction Index")) {

        try {
          unsigned long t = stoul(goto_text);
          if (t >= local_vm->ins_max)
            t = local_vm->ins_max;

          if (t > local_vm->ins_i) {
            while (local_vm->ins_i < t) {
              if (local_vm->step() == false) {
                done = true;
                break;
              }
            }
          }
        } catch (...) {
          error = 60 * 5;  // Five seconds
        }
      }
      if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
        ImGui::SetTooltip("Move forward to specified instruction index.");
      }

      ImGui::SameLine();
      if (ImGui::Button("Instruction")) {
        if (!goto_text.empty() &&
            !(goto_text[0] >= '0' && goto_text[0] <= '9')) {
          std::cout << "TEST" << std::endl;
          char gt = goto_text[0];
          while (local_vm->code[local_vm->ins_i] != gt) {
            if (local_vm->step() == false) {
              done = true;
              break;
            }
          }
        }
      }
      if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
        ImGui::SetTooltip("Move forward to first specified instruction.");
      }

      ImGui::SameLine();
      if (ImGui::Button("Step")) {
        try {
          unsigned long t = stoul(goto_text);
          if (t > local_vm->total_steps) {
            while (local_vm->total_steps < t) {
              if (local_vm->step() == false) {
                done = true;
                break;
              }
            }
          }
        } catch (...) {
          error = 60 * 5;  // Five seconds
        }
      }
      if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
        ImGui::SetTooltip("Move forward to specified step count.");
      }

      if (error != 0) {
        error--;
        ImGui::SameLine();
        ImGui::Text("Invalid input format.");
      }
    }

    ImGui::Text("Steps: %d", local_vm->total_steps);
    ImGui::Text("Instruction Index: %d", local_vm->ins_i);
    output(true);

    ImGui::End();
  }

  void frontend(VM& vm) {
    if (Graphics::setup() != true)
      return;
    local_vm = &vm;
    Graphics::mainFuncAdd(graphical_loop);

    while (Graphics::main() == true) {}

    Graphics::End::end();
    local_vm = nullptr;
  }
}
// }}}
// GraphicalFrontend {{{
namespace GraphicalFrontend {
  namespace {  // <anonymous>
    VM* local_vm = nullptr;
    bool done = false;
    unsigned int* heat_map;
  }

  void destruct() {
    delete[] heat_map;
  }

  static void advanceLocalVM() {
    if (local_vm->step() == false)
      done = true;
    heat_map[local_vm->pc]++;
  }

  static void menu() {
    if (!ImGui::BeginMenuBar())
      return;

    if (ImGui::BeginMenu("Main")) {
      if (ImGui::MenuItem("quit"))
        Graphics::kill_me = true;
      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }

  static void bufferView() {
    ImGui::Text("Buffer View:");

    static int len = 5;
    for (int y = 0, z = 0; y < len; y++) {
      for (int x = 0; x < len; x++, z++) {
        if (x > 0)
          ImGui::SameLine();
        ImGui::PushID((y * len + x) + 25);

        int i = local_vm->buffer[z];
        char buf[10]{};
        if (i != 0)
          std::to_chars(buf, buf + 10, i);
        ImGui::Selectable(buf, i != 0, 0, ImVec2(25, 25));
        ImGui::PopID();
      }
    }
  }

  static void heatMapView() {
    ImGui::Text("Buffer Access Heat Map:");

    static int len = 5;
    for (int y = 0, z = 0; y < len; y++) {
      for (int x = 0; x < len; x++, z++) {
        if (x > 0)
          ImGui::SameLine();
        ImGui::PushID((y * len + x) + 50);

        int i = heat_map[z];
        char buf[10]{};
        if (i != 0)
          std::to_chars(buf, buf + 10, i);
        ImGui::Selectable(buf, true, 0, ImVec2(25, 25));
        ImGui::PopID();
      }
    }
  }

  static void instructionView() {
    ImGui::Text("Instruction List:");

    // TODO: make inspect_instruction call a function that accepts a function
    // instaed, then use that function.
    const unsigned int sub = 4;
    const unsigned int cap = local_vm->ins_max;
    unsigned int frame_len = 9;
    unsigned int len = cap;
    unsigned int pc = local_vm->ins_i;
    unsigned int start;

    if (len < frame_len)
      frame_len = len;

    if (pc <= sub)
      start = 0;
    else
      start = pc - sub;

    unsigned int end = start + frame_len;
    if (end > cap)
      end = cap;

    ImGui::PushFont(NULL, 20.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
    for (unsigned int x = start; x < end; x++) {
      if (x != start)
        ImGui::SameLine();
      ImGui::PushID(x);

      char buf[2]{' ', 0};
      buf[0] = local_vm->code[x];
      if (local_vm->ins_i == x) {
        // Shift Selectable Posistion
        ImVec2 pos = ImGui::GetCursorPos();
        ImGui::SetCursorPos(ImVec2(pos.x, pos.y - 5.0f));

        // Boarder Posistions
        ImVec2 bpos = ImGui::GetCursorScreenPos();
        ImVec2 size = ImVec2(35, 35);
        ImVec2 bpos2 = ImVec2(bpos.x + size.x, bpos.y + size.y);

        ImGui::Selectable(buf, true, 0, size);

        ImU32 border_color = IM_COL32(0, 0, 200, 255);
        ImGui::GetWindowDrawList()->AddRect(bpos, bpos2, border_color, 0.0f, 0,
                                            5.0f);
      } else {
        ImGui::Selectable(buf, true, 0, ImVec2(25, 25));
      }
      ImGui::PopID();
    }
    ImGui::PopStyleVar();
    ImGui::PopFont();
  }

  void graphical_loop(Graphics::Main_Function&) {
    ImGui::Begin("Graphical Frontend", nullptr, ImGuiWindowFlags_MenuBar);

    menu();

    bufferView();
    heatMapView();

    if (done == true) {
      ImGui::End();
      return;
    }

    static int n = 0;
    if (n++ > 1) {
      advanceLocalVM();
      n = 0;
    }

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    instructionView();

    ImGui::End();
  }

  void frontend(VM& vm) {
    if (Graphics::setup() != true)
      return;
    local_vm = &vm;
    Graphics::mainFuncAdd(graphical_loop);

    unsigned int len = vm.rules.tape_length;
    heat_map = new unsigned int[len];
    memset(heat_map, 0, len * sizeof(unsigned int));
    Unwind::add_unwind({.fptr = destruct, .name = ""});

    while (Graphics::main() == true) {}

    Graphics::End::end();
    local_vm = nullptr;
  }
}
// }}}

// Frontend {{{
namespace Frontend {
#define CONFIG FRONTEND_CONFIG
#define CONF \
  CONFIG     \
  X(_LEN, nullptr)

  enum Frontend_Index {
#define X(A, B, ...) A,
    CONF
#undef X
  };

  struct Functions {
    const std::string_view name;
    void (*const func)(VM&);
  };

  const char* name[] = {
#define X(A, B, ...) STR(A),
      CONFIG
#undef X
  };

  const struct Functions functions[] = {
#define X(A, B, ...) {.name = STR(A), .func = B},
      CONFIG
#undef X
  };

#undef CONF
#undef CONFIG

  const Log::O error = {
      .e = true,
      .lm = Log::FRONTEND,
  };

  void frontend(Backend& backend, enum Frontend_Index fi) {
    if (backend.state != Backend::EXE) {
      Log::print(error,
                 "Program: Frontend self-incrementing of backend status not "
                 "supported.\n Will need to increment status to Execution.");
      exit(1);
    }

    VM vm(backend.export_vm());

    functions[fi].func(vm);
  }
};
// }}}

#endif
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
