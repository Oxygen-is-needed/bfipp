#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <limits>
#include <print>
#include <stack>
#include <string>
#include <vector>

#include "config.hpp"
#include "utils.hpp"
#include "log.hpp"
#include "escape.hpp"

#include "graphics.hpp"
#include "backend.hpp"
#include "frontend.hpp"
#include "output.hpp"


// Args {{{
namespace Args {
  // {Anonymous} {{{
  namespace {
#define KEYS  PROGRAM_FLAGS

    struct option longopts[] = {
#define X(A, F, G, B, C, E) {STR(B), C, nullptr, A},
      KEYS
#undef  X
    };

    const char opts[] = {
#define X(A, F, G, B, C, E) A, F, G,
        KEYS
#undef X
    };

    void frontend_help() {
      void (*const funcs[])() = {
#define X(A, B, C, ...) C,
          FRONTEND_CONFIG
#undef X
      };

      for (int x=0; x<FRONTEND_LENGTH; x++) {
        if (funcs[x] == nullptr)
          continue;
        funcs[x]();
      }
    }

    void exec_help() {
#define X(A, F, G, B, C, E) Utils::print_help(A, STR(B), E);
      KEYS
#undef  X
    }

    namespace Keys {
      enum {
#define X(A, F, G, B, ...)  B,
        KEYS
#undef  X
      };
      int enumerate_key(int key) {

        switch(key) {
#define X(A, F, G, B, ...)  case A: return Keys::B ;
          KEYS
#undef  X
          default: return -1;
        }
      }
    }

#undef KEYS
  }
  // }}}

  void print_version() {
    std::cout << "bfi++ " << VERSION << " (" << __DATE__ << ", "
      << __TIME__ << ") [" << STR(CC) << " " << __VERSION__
      << "]" << std::endl;
  }

  enum Get_Status {
    NONE, FILE, TEXT, HELLO
  };

  bool frontend_run = true;
  bool graphical = false;
  enum Frontend::Frontend_Index frontend = Frontend::SIMPLE_TEXT;
  enum Get_Status gstat=NONE;
  enum Output::Method output = Output::__NONE__;
  std::filesystem::path output_file = "";
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
    const std::string_view description[] = {
#define X(A,B,C,D,...)  D,
      FRONTEND_CONFIG
#undef  X
    };

    Log::print(verbose, "Listing available frontends.\n\tNOTE: case does not "
                        "matter when choosing one.");

    std::cout << FRONTEND_DESCRIPTION "\n";
    for (int x=0; x<FRONTEND_LENGTH; x++) {
      if (Frontend::functions[x].func == nullptr)
        continue;
      if (!description[x].empty()) {
        std::println("\t{:2}| {:16} - {}", x+1, Frontend::functions[x].name, description[x]);
        continue;
      }

      std::println("\t{:2}| {:16}", x+1, Frontend::functions[x].name);
    }
  }

  bool check_frontend(int num) {
    if (Frontend::functions[num].func == nullptr) {
      return false;
    }

    frontend = static_cast<Frontend::Frontend_Index>(num);
    return true;
  }

  bool choose_frontend(std::string method) {
    try {
      int num = std::stoi(method);
      if (num > 0 && num <= FRONTEND_LENGTH) {
        return check_frontend(num-1);
      }
    } catch (std::invalid_argument& e) {
    }

    std::transform(method.begin(), method.end(), method.begin(), ::toupper);

    for (int x=0; x<FRONTEND_LENGTH; x++) {
      if (Frontend::functions[x].name == method) {
        return check_frontend(x);
      }
    }

    return false;
  }

  void parse(int opt, char* arg) {
    switch (opt) {
      // RUN AND KILL
      case Keys::lhelp:
        exec_help();
        frontend_help();
        exit(0);
      case Keys::help:
        exec_help();
        exit(0);
      case Keys::version:
        print_version();
        exit(0);
        break;

        // INPUT
      case Keys::file:
        gstat = FILE;
        if (arg == nullptr) {
          Log::print(error, "Optarg was set to null. No file was provided.");
          exit(1);
        }
        text = arg;
        Log::print(verbose, "Enabled input from file");
        break;
      case Keys::input:
        if (arg == nullptr) {
          gstat = HELLO;
          Log::print(verbose, "Set input to Hello World default");
          break;
        }
        gstat = TEXT;
        text = arg;
        if (text == "-") {
          Log::print(verbose, "Reading from stdin for input");
          std::cin >> text;
          break;
        }
        Log::print(verbose, "Set argument as input");
        break;

        // FRONTEND
      case Keys::run:
        frontend = Frontend::NONE;
        Log::print(verbose, "Enabled run mode");
        break;
      case Keys::frontend:
        if (arg != nullptr) {
          if (choose_frontend(arg) == false) {
            Log::print(error, "Unable to find frontend method");
            exit(1);
          }
          Log::print(verbose, "Changed frontend to ",
              Frontend::functions[frontend].name);
          break;
        }
        [[fallthrough]];
      case Keys::list_fronts:
        list_frontends();
        exit(0);
        break;

        // OUTPUT
      case Keys::only_output:
        frontend_run = false;

        [[fallthrough]];
      case Keys::output:
        if (arg == nullptr) {
          Log::print(error, "Unable to find output file");
          exit(1);
        }

        // TODO: check if already changed
        output = Output::RAW;
        output_file = arg;
        break;

        // OTHER
      case Keys::verbose:
        Log::verbose_level = Log::verbose_max;
        Log::print(verbose, "Enabled Verbose");
        break;
      case Keys::gui:
        graphical = true;
        break;
      case -1:
        Log::print(error, "Unknown Option");
        exit(1);
    }
  }

  void arguments(int argc, char* argv[]) {
    int opt = 0;
    while ((opt = getopt_long(argc, argv, opts, longopts, nullptr)) != -1) {
      if (opt == '?')
        exit(1);
      parse(Keys::enumerate_key(opt), optarg);
    }
  }

  void get_input(Backend& bf) {
    switch(gstat) {
      case NONE:
        Log::print(error,"No code input specified.");
        exec_help();
        exit(1);
      case FILE:
        if (use_text() == false)
          exit(1);
        if (!std::filesystem::exists(text)) {
          Log::print(Log::error(), "File does not exist.");
          exit(1);
        }
        Log::print(verbose, "Getting data from file");
        bf.get_file(text);
        break;
      case TEXT:
        if (use_text() == false)
          exit(1);
        Log::print(verbose, "Getting data from input");
        bf.get(text);
        break;
      case HELLO:
        bf.get();
        break;
    }
  }
}
// }}}
// Graphical {{{
namespace Graphical {
#ifndef DISABLE_GRAPHICS
  void warningPopup(Graphics::Main_Function&) {
    static int popup = true;
    if (popup == true) {
      ImGui::OpenPopup("Warning !");
      popup = false;
    }

    if (ImGui::BeginPopupModal("Warning !")) {
      ImGui::Text("This is currently not a graphical application.\n");
      ImGui::TextWrapped("This program should be used in the terminal."
          "For the documentation for this program please visit:");
      ImGui::TextLinkOpenURL("https://github.com/Oxygen-is-needed/bfipp", NULL);

      if (Graphics::centerButton("Ok"))
        Graphics::kill_me = true;

      if (Graphics::centerButton("Load \"Hello World\""))
        Args::parse(Args::Keys::input, nullptr);

      ImGui::EndPopup();
    }
  }

  void menu() {
    // Setup
    if (Graphics::setup() != true)
      return;

    // Main loop
    Graphics::mainFuncAdd(warningPopup);
    Graphics::Main_Loop_Status status = Graphics::END;
    while ((status = Graphics::main()) == Graphics::CONTINUE) ;

    // Cleanup
    Graphics::End::end();
  }
#else
  void menu() {
    Log::print(Args::error, "Graphical menu is Disabled");
  }
#endif
}
// }}}

int main(int argc, char* argv[]) {
  Unwind::initalize_unwind();
  Args::arguments(argc, argv);
  if (Args::graphical == true) {
    Graphical::menu();
  }

  // Set Settings
  Backend bf;
  if (Log::verbose_level == 2) {
    bf.print_rules();
  }

  // Get input
  Args::get_input(bf);

  // Convert
  Log::print({.v=1,.lm=Log::BACKEND},"Converting to IR representation");
  bf.convert();

  // Execute
  if (!Args::output_file.empty())
    Output::output(bf, Args::frontend, Args::output_file, Args::output);

  // Frontend
  if (Args::frontend_run == false)
    return 0;
  Log::print({.v=1,.lm=Log::FRONTEND}, "Running frontend");
  Frontend::frontend(bf, Args::frontend);
}
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
