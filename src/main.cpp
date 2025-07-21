#include <filesystem>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <limits>
#include <print>
#include <stack>
#include <string>
#include <vector>
#include <cctype>

#include "config.hpp"
#include "utils.hpp"
#include "log.hpp"
#include "escape.hpp"

#include "backend.hpp"
#include "frontend.hpp"

// Args {{{
namespace Args {
  namespace {
#define KEYS  PROGRAM_FLAGS

    struct option longopts[] = {
#define X(A,F,G,B,C,E)  { B, C, nullptr, A },
      KEYS
#undef  X
    };

    const char opts[] = {
#define X(A,F,G,B,C,E)  A, F, G,
      KEYS
#undef  X
    };

    namespace {
      void exec_help_helper(const int &opt, const std::string_view &flag,
          const std::string_view &msg){
        std::cout << "\t" << "-" << (char)opt << " | --" << flag << "\t - " << msg
          << std::endl;
        return;
      }
    }

    void exec_help() {
#define X(A, F, G, B, C, E) exec_help_helper(A, B, E);
      KEYS
#undef  X
    }

#undef KEYS
  }

  void print_version() {
    std::cout << "bfi++ " << VERSION << " (" << __DATE__ << ", "
      << __TIME__ << ") [" << STR(CC) << " " << __VERSION__
      << "]" << std::endl;
  }

  enum Get_Status {
    NONE, FILE, TEXT, HELLO
  };
  enum Get_Status gstat=NONE;
  enum Frontend::Frontend_Index frontend = Frontend::TERM_SIMPLE;
  std::string text = "";

  Log::O verbose = {
    .v = 1,
    .lm = Log::SETTINGS,
  };
  Log::O error = {
    .e = true,
    .lm = Log::SETTINGS
  };

  bool use_text() {
    if (text.empty() == true) {
      Log::print(error, "No code input specified.");
      return false;
    }
    return true;
  }

  void list_frontends() {
    Log::print(verbose, "Listing available frontends.\n\tNOTE: case does not "
                        "matter when choosing one.");
    for (int x=0; x<FRONTEND_LENGTH; x++) {
      std::println("\t{:2}| {}", x+1, Frontend::functions[x].name);
    }
  }

  bool choose_frontend(std::string method) {
    try {
      int num = std::stoi(method);
      if (num > 0 && num <= FRONTEND_LENGTH) {
        frontend = static_cast<Frontend::Frontend_Index>(num-1);
        return true;
      }
    } catch (std::invalid_argument& e) {
    }

    std::transform(method.begin(), method.end(), method.begin(), ::toupper);

    for (int x=0; x<FRONTEND_LENGTH; x++) {
      if (Frontend::functions[x].name == method) {
        frontend = static_cast<Frontend::Frontend_Index>(x);
        return true;
      }
    }

    return false;
  }

  // TODO: check optarg does not equal nullptr
  void arguments(int argc, char* argv[]) {
    int opt = 0;
    while ((opt = getopt_long(argc, argv, opts, longopts, nullptr)) != -1) {
      switch (opt) {
        // RUN AND KILL
        case 'h':
          exec_help();
          exit(0);
          break;
        case 'v':
          exit(0);
          break;

          // INPUT
        case 'f':
          gstat = FILE;
          text = optarg;
          Log::print(verbose, "Enabled input from file");
          break;
        case 'i':
          if (optarg == nullptr) {
            gstat = HELLO;
            Log::print(verbose, "Set input to Hello World default");
            break;
          }
          gstat = TEXT;
          text = optarg;
          if (text == "-") {
            Log::print(verbose, "Reading from stdin for input");
            std::cin >> text;
            break;
          }
          Log::print(verbose, "Set argument as input");
          break;

          // FRONTEND
        case 'r':
          frontend = Frontend::NONE;
          Log::print(verbose, "Enabled run mode");
          break;
        case 'F':
          if (optarg != nullptr) {
            if (choose_frontend(optarg) == false) {
              Log::print(error, "Unable to find frontend method");
              exit(1);
            }
            Log::print(verbose, "Changed frontend to ",
                       Frontend::functions[frontend].name);
            break;
          }
          [[fallthrough]];
        case 'l':
          list_frontends();
          exit(0);
          break;

          // OTHER
        case 'V':
          // TODO: add optional argument to set level of verbose
          Log::verbose_level = Log::verbose_max;
          Log::print(verbose, "Enabled Verbose");
          break;
      }
    }
  }
}
// }}}

int main(int argc, char* argv[]) {
  Unwind::initalize_unwind();
  Args::arguments(argc, argv);

  // Set Settings
  Backend bf;

  // Get input /* Ignoring non program charactors */
  switch(Args::gstat) {
    case Args::NONE:
      Log::print(Log::error(),"No code input specified.");
      Args::exec_help();
      return 0;

    case Args::FILE:
      if (Args::use_text() == false) return 0;
      if (!std::filesystem::exists(Args::text)) {
        Log::print(Log::error(), "File does not exist.");
        return 0;
      }
      Log::print(Args::verbose, "Getting data from file");
      bf.get_file(Args::text);
      break;
    case Args::TEXT:
      if (Args::use_text() == false) return 0;

      Log::print(Args::verbose, "Getting data from input");
      bf.get(Args::text);
      break;
    case Args::HELLO:
      bf.get();
      break;
  }

  // Convert to IR
  Log::print({.v=1,.lm=Log::BACKEND},"Converting to IR representation");
  bf.convert(Backend::C_IR);

  // Execute (Option for stepping through)
  Log::print({.v=1,.lm=Log::FRONTEND}, "Running frontend");
  Frontend::frontend(bf, Args::frontend);
}
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
