#include <filesystem>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <limits>
#include <stack>
#include <string>
#include <vector>

#include "backend.hpp"
#include "frontend.hpp"

// Args {{{
namespace Args {
  namespace {
    // TODO: move to config file
    // NOTE: edit to configure the arguments and help page text.
#define KEYS  \
    X('f', ':', ' ', "file",    required_argument,  nullptr,  "file: Input file.")    \
    X('h', ' ', ' ', "help",    no_argument,        nullptr,  "Prints helps text.")   \
    X('i', ':', ':', "input",   required_argument,  nullptr,  "[txt]: Input text.\n\t\tDefault with no arguments.\n\t\t'-' for reading from stdin.")   \
    X('r', ' ', ' ', "run",     no_argument,        nullptr,  "Run input imediatly.") \
    X('V', ' ', ' ', "verbose", no_argument,        nullptr,  "Enable verbose output.") \
    X('v', ' ', ' ', "version", no_argument,        nullptr,  "Print version")

    struct option longopts[] = {
#define X(A,F,G,B,C,D,E)  { B, C, D, A },
      KEYS
#undef  X
    };
    const char opts[] = {
#define X(A,F,G,B,C,D,E)  A, F, G,
      KEYS
#undef  X
    };

    void exec_help() {
#define X(A,F,G,B,C,D,E)  std::cout << "\t" << "-" << A \
      << " | " << "--" << B << "\t - " << E << std::endl;
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

  bool use_text() {
    if (text.empty() == true) {
      Log::print({.e=true,.lm=Log::SETTINGS}, "No code input specified.");
      return false;
    }
    return true;
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
