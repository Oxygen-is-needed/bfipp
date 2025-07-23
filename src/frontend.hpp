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
}
// }}}

// None {{{
namespace None {
  void frontend(Backend& backend) {
    VM& got_vm = backend.get_vm();
    got_vm.run();

    Frontend_Utils::print_output(got_vm.output);
  }
}
// }}}
// SimpleTextFrontend {{{
namespace SimpleTextFrontend {
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
  void keybindings(char &key, bool &ret, bool &ug, int &phelp,
      unsigned int &skip, unsigned int &wait) {
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
    }
  }

  void inspect_instructions(Backend& backend, unsigned int cap, unsigned int pc) {
    unsigned int len = cap;
    unsigned int frame_len = 9;
    unsigned int sub = 4;
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
          << C::YELLOW << backend.code[x]
          << C::BLUE << "]" << C::RESET;
        continue;
      }
      std::cout
        << C::BLUE << "[" << C::RESET
        << backend.code[x]
        << C::BLUE << "]" << C::RESET;
    }
    std::cout << "\n   ";
    for (unsigned int x=start; x<end; x++) {
      std::cout << "`" << x%9+1 << "`";
    }
    std::cout << std::endl;
  }

  std::string out = "";

  void unwind() {
    Frontend_Utils::print_output(out);
    C::show_cursor();
  }

  void frontend(Backend& backend) {
    VM& got_vm = backend.get_vm();
    out = got_vm.output;

    got_vm.inspect_buffer();
    inspect_instructions(backend, got_vm.ins_max, got_vm.ins_i);
    bool user_guided = true;
    bool ret = true;
    int phelp = false;
    unsigned int skip_i = 0;
    unsigned int skip = 0;
    unsigned int wait = 0;
    C::hide_cursor();
    Unwind::add_unwind({unwind, "SimpleTextFrontend::unwind"});
    while(ret != false) {
      if (user_guided == true && skip_i++ >= skip
          && (wait == 0 || wait < got_vm.ins_i)) {
        skip_i = 0;
        char in = getchar();
        std::string input;

        keybindings(in, ret, user_guided, phelp, skip, wait);
        if (ret == false)
          continue;

        C::clear();

        got_vm.inspect_buffer(-1);
        inspect_instructions(backend, got_vm.ins_max, got_vm.ins_i);
        std::cout << "\nOutput: " << got_vm.output;

        if (phelp > 0) {
          help();
          phelp--;
        }

        std::cout << std::endl;

      }
      ret = got_vm.step();
    }
    C::show_cursor();

    Frontend_Utils::print_output(got_vm.output);
    Unwind::pop_unwind();
  }

#undef KEYS
}
// }}}

// Frontend {{{
namespace Frontend {
#define CONFIG  FRONTEND_CONFIG

  /**
   * define CONF - adds _LEN enum, to show amount in enum.
   */
#define CONF  \
  CONFIG  \
  X(_LEN, nullptr)


  enum Frontend_Index {
#define X(A,B,...)  A,
    CONF
#undef  X
  };

  struct Functions {
    const std::string_view name;
    void (*const func)(Backend&);
  };
  struct Functions functions[] = {
#define X(A,B,...)  { .name = STR(A) , .func = B },
    CONFIG
#undef  X
  };

  void execution_loop(Backend& backend, enum Frontend_Index fi) {
    backend.execute_set();

    functions[fi].func(backend);
  }
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

    execution_loop(backend, fi);
  }
};
// }}}

#endif
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
