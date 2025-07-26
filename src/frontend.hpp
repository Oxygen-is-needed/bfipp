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

  unsigned int inspect_buffer(VM& vm, ssize_t mwidth=100, int view_frame=14) {
    if (mwidth == -1) {
      mwidth = std::numeric_limits<int>::max();
    }

    int width=view_frame;
    unsigned int out=5;
    std::cout << vm.total_steps << ": ";
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
        std::cout << C::B << "{" << C::Y
          << code
          << C::B << "} " << C::RESET;
        continue;
      }

      std::cout << "[" << code << "] ";
    }
    std::cout << std::endl;
    out += 4*y;
    return out;
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

  void help() {
      std::cout << "\n" FRONTEND_DESCRIPTION__SIMPLE_TEXT "\n";
#define X(A,B,C)  Utils::print_help(B, "", C, false);
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

  void inspect_instructions(VM& vm) {
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


    std::cout << "I: ";
    for (unsigned int x=start; x<end; x++) {
      if (x == pc) {
        std::cout
          << C::BLUE << "["
          << C::YELLOW << vm.code[x]
          << C::BLUE << "]" << C::RESET;
        continue;
      }
      std::cout
        << C::BLUE << "[" << C::RESET
        << vm.code[x]
        << C::BLUE << "]" << C::RESET;
    }
    std::cout << "\n   ";
    for (unsigned int x=start; x<end; x++) {
      std::cout << "`" << x%10 << "`";
    }
    std::cout << " +" << (pc/10)*10 << std::endl;
  }

  std::string out = "";

  void unwind() {
    Frontend_Utils::print_output(out);
    C::show_cursor();
  }

  void frontend(VM& vm) {
    out = vm.output;

    Frontend_Utils::inspect_buffer(vm);
    inspect_instructions(vm);

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
        inspect_instructions(vm);
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
