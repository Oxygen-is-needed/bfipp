#ifndef FRONTEND__HPP
#define FRONTEND__HPP

// Frontend Utilities {{{
namespace Frontend_Utils {
  void print_output(std::string output) {
    if (output.empty())
      return;

    std::cout << C::M << "\nOUTPUT =====\n" << C::RESET
      << output
      << C::M << "\n============" << C::RESET << std::endl;
    fflush(stdout);
  }

  // TODO: make easier
  std::string ic_default[3] = {
    "\033[" STR(M_BLUE) "m",
    "\033[" STR(M_YELLOW) "m",
    "\033[" STR(M_RESET) "m",
  };

  std::string ic_none[3] = {"", "", ""};

  unsigned int inspect_buffer(VM &vm, ssize_t mwidth = 100, int view_frame = 14,
                              std::ostream &os = std::cout,
                              std::string (&ic)[3] = ic_default) {

    if (mwidth == -1) {
      mwidth = std::numeric_limits<int>::max();
    }

    int width=view_frame;
    unsigned int out=5;
    os << vm.total_steps << ": ";
    unsigned int y=0;
    for (int x=0; x<width && y<mwidth && y<vm.rules.tape_length; x++,y++) {
      if (vm.buffer[y] != 0) {
        x=0;
      }

      std::string code = "";
      if (vm.buffer[y] != 0) {
        if (isprint(vm.buffer[y]) == 0 || vm.buffer[y] == ' ') {
          code = std::to_string(vm.buffer[y]);
        } else  {
          code += vm.buffer[y];
        }
      }

      if (vm.pc == y) {
        os << ic[0] << "{" << ic[1]
          << code
          << ic[0] << "} " << ic[2];
        continue;
      }

      os << "[" << code << "] ";
    }
    os << std::endl;
    out += 4*y;
    return out;
  }

  void inspect_instructions(VM& vm, std::ostream& os=std::cout, std::string (&ic)[3] = ic_default) {
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

    unsigned int end = start+frame_len;
    if (end > cap)
      end = cap;


    os << "I: ";
    for (unsigned int x=start; x<end; x++) {
      if (x == pc) {
        os
          << ic[0] << "("
          << ic[1] << vm.code[x]
          << ic[0] << ")" << ic[2];
        continue;
      }
      os
        << ic[0] << "[" << ic[2]
        << vm.code[x]
        << ic[0] << "]" << ic[2];
    }
    os << "\n   ";
    for (unsigned int x=start; x<end; x++) {
      os << "`" << x%10 << "`";
    }
    os << " +" << (pc/10)*10 << std::endl;
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
#define KEYS  FRONTEND_KEYS__SIMPLE_TEXT

  enum Keybinds {
#define X(A,B,C) A = B,
    KEYS
#undef X
  };

  void help(std::ostream &s = std::cout) {
    s << "\n" FRONTEND_DESCRIPTION__SIMPLE_TEXT "\n";
#define X(A, B, C) Utils::print_help(B, "", C, false, s);
    KEYS
#undef X
  }

  unsigned int get_int() {
    std::string input;
    std::cin >> input;
    return static_cast<unsigned int>
      (std::stoul(input));
  }
  void keybindings(VM& vm, char &key, bool &ret, bool &ug, int &phelp,
      unsigned int &skip, unsigned int &wait, char& wi) {
    switch(key) {
      case QUIT:
        ret = false; return;
      case HELP:
        phelp = 2; return;

      case RUN:
        ug = false; return;
      case SKIP:
        skip = get_int();
        return;
      case WAIT:
        wait = get_int();
        return;
      case INSTRUCTION:
        wi = getchar();
        return;
      case CLEAR:
        vm.output.clear();
        return;
    }
  }

  std::string out = "";

  void unwind() {
    Frontend_Utils::print_output(out);
    C::show_cursor();
  }

  void frontend(VM& vm) {
    out = vm.output;

    Frontend_Utils::inspect_buffer(vm);
    Frontend_Utils::inspect_instructions(vm);

    bool ret = true;
    bool user_guided = true;
    char wait_instruction = 0;
    int phelp = false;
    unsigned int skip = 0;
    unsigned int skip_i = 0;
    unsigned int wait = 0;

    C::hide_cursor();
    Unwind::add_unwind({unwind, "SimpleTextFrontend::unwind"});
    while(ret != false) {
      if (user_guided == true && skip_i++ >= skip &&
          (wait == 0 || wait < vm.ins_i) &&
          (wait_instruction == 0 ||
           (vm.ins_i < vm.ins_max && wait_instruction == vm.code[vm.ins_i]))) {
        skip_i = 0;
        wait_instruction = 0;
        char in = getchar();
        std::string input;

        keybindings(vm, in, ret, user_guided, phelp, skip, wait, wait_instruction);
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
  namespace { // <anonymous>
    VM* local_vm = nullptr;
    bool done = false;
  }

  static void menu() {
    if (!ImGui::BeginMenuBar())
      return;

    if (ImGui::BeginMenu("Main")) {
      if (ImGui::MenuItem("quit"))
        Graphics::kill_me = true;
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Options")) {
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Info")) {
      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }

  void glSteps(const int& mult) {
        for (int x=0; x<mult; x++) {
          if (local_vm->step() != false)
            continue;
          done = true;
          break;
        }
  }

  void graphical_loop(Graphics::Main_Function&) {
    ImGui::Begin("Hello, world!", nullptr, ImGuiWindowFlags_MenuBar);

    menu();

    ImGui::Text("This is a proof of concept for A simple Graphical Frontend.");
    ImGui::TextWrapped(
        "It proves that setting the frontend then passing to the "
        "graphical loop to the frontend can work.");
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    static std::string buf = "";
    ImGui::TextUnformatted(buf.c_str());

    if (done == true) {
      ImGui::TextUnformatted(local_vm->output.c_str());
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
      // TODO: move to graphics file
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

      ImGui::SliderInt("Step Size", &step_mult, 1, 10);
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

    ImGui::Text("Steps: %d", local_vm->total_steps);
    ImGui::Text("Output:\n%s", local_vm->output.c_str());
    ImGui::End();
  }

  void help(/*std::ostream &s = std::cout*/) {

  }
  void frontend(VM& vm) {
    if (Graphics::setup() != true)
      return;
    local_vm = &vm;
    Graphics::mainFuncAdd(graphical_loop);

    while (Graphics::main() == true) ;

    Graphics::End::end();
    local_vm = nullptr;
  }
}
// }}}

// Frontend {{{
namespace Frontend {
#define CONFIG  FRONTEND_CONFIG
#define CONF                                                                   \
  CONFIG                                                                       \
  X(_LEN, nullptr)

  enum Frontend_Index {
#define X(A,B,...)  A,
    CONF
#undef  X
  };

  struct Functions {
    const std::string_view name;
    void (*const func)(VM&);
  };
  const struct Functions functions[] = {
#define X(A,B,...)  { .name = STR(A) , .func = B },
    CONFIG
#undef  X
  };

#undef  CONF
#undef  CONFIG


  const Log::O error = {
    .e = true,
    .lm = Log::FRONTEND,
  };

  void frontend(Backend& backend, enum Frontend_Index fi) {
    if (backend.state != Backend::EXE) {
      Log::print(error,
          "Program: Frontend self-incrementing of backend status not supported.\n"
          "Will need to increment status to Execution.");
      exit(1);
    }

    VM vm(backend.export_vm());

    functions[fi].func(vm);
  }
};
// }}}

#endif
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
