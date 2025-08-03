#ifndef ESCAPE__HPP
#define ESCAPE__HPP

// Unwinding {{{
namespace Unwind {
  namespace {
    Log::O verbose = {
      .v = 2,
    };

    struct Unwind {
      void (*fptr)();
      std::string name;
    };
    std::vector<Unwind> unwind_vector;
    bool unwind_b=false;
  }

  void pop_unwind(void (*fptr)()) {
    for (int x=0; x<static_cast<int>(unwind_vector.size());) {
      if (unwind_vector[x].fptr == fptr) {
        Log::print(verbose, "Popping: ", unwind_vector[x].name);
        unwind_vector.erase(unwind_vector.begin() + x);
        continue;
      }
      ++x;
    }
  }
  void pop_unwind() {
    unwind_vector.erase(--unwind_vector.end());
  }
  namespace {
    void run_unwind() {
      if (unwind_vector.empty() == true || unwind_b == true) {
        return;
      }
      for (int x=unwind_vector.size()-1; x>=0; x--) {
        Log::print(verbose, "Unwinding: ", unwind_vector[x].name);
        unwind_vector[x].fptr();
        pop_unwind();
      }
    }
  }

  void add_unwind(struct Unwind u) {
    unwind_vector.push_back(u);
  }
  void unwind() {
    run_unwind();
    exit(0);
  }

  namespace {
    const Log::O error = {
      .e = true,
      .lm = Log::NONE
    };

    static int used_signal = 0;
    void if_used(int sig, const std::string& name) {
      if (used_signal == sig) {
        Log::print(error, "Caught multiple ", name, " signals. Exiting Immediately.");
        exit(sig);
      }

      used_signal = sig;
    }

    void handle_sigint(int sig) {
      switch (sig) {
        case SIGINT:
          if_used(sig, "SIGINT");
          Log::print({}, "Received SIGINT (Ctrl+C). Unwinding then Exiting.");
          break;
        case SIGTERM:
          if_used(sig, "SIGTERM");
          Log::print(error, "Received SIGTERM. Unwinding then Exiting.");
          break;

        case SIGSEGV:
          if_used(sig, "SIGSEGV");
          Log::print(error, "Received SIGSEGV. Unwinding then Exiting.");
          break;

        default:
          if_used(sig, "UNKNOWN");
          Log::print(error, "Received Unknown Signal. Unwinding then Exiting.");
      }
      run_unwind();
      exit(sig);
    }
  }
  void signal_handler() {
#ifdef _WIN32
    signal(SIGINT,  handle_sigint);
    signal(SIGTERM, handle_sigint);
    signal(SIGABRT, handle_sigint);
#else
    struct sigaction sa;

    sa.sa_handler   = &handle_sigint;
    sa.sa_flags     = SA_RESTART;

    sigaction(SIGINT,   &sa, nullptr);
    sigaction(SIGTERM,  &sa, nullptr);
    sigaction(SIGSEGV,  &sa, nullptr);
#endif
  }

  void on_exit() {
    if (unwind_b == true)
      return;
    run_unwind();
  }
  void initalize_unwind() {
    signal_handler();
    atexit(on_exit);
  }
}
// }}}

#endif
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
