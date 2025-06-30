#include <vector>
#include <string>
#include <iostream>
#include <getopt.h>

#include "backend.cpp"
#include "frontend.cpp"

// Args {{{
namespace Args {
  namespace {
    // TODO: move to config file
    // NOTE: edit to configure the arguments and help page text.
#define KEYS  \
    X('f', ':', ' ', "file",    required_argument,  NULL, "file: Input file.")    \
    X('h', ' ', ' ', "help",    no_argument,        NULL, "Prints helps text.")   \
    X('i', ':', ':', "input",   required_argument,  NULL, "[txt]: Input text.\n\t\tDefault with no arguments.\n\t\t'-' for reading from stdin.")   \
    X('r', ' ', ' ', "run",     no_argument,        NULL, "Run input imediatly.") \
    X('V', ' ', ' ', "verbose", no_argument,        NULL, "Enable verbose output.") \
    X('v', ' ', ' ', "version", no_argument,        NULL, "Print version")

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

    void exec_help(void) {
#define X(A,F,G,B,C,D,E)  std::cout << "\t" << "-" << A \
      << " | " << "--" << B << "\t - " << E << endl;
      KEYS
#undef  X
    }

#undef KEYS
  }
  
  void print_version(void) {
    std::cout << "bfi++ " << VERSION << " (" << __DATE__ << ", "
              << __TIME__ << ") [" << STR(CC) << " " << __VERSION__
              << "]" << endl;
  }

  enum Get_Status {
    NONE, FILE, TEXT, HELLO
  };
  enum Get_Status gstat=NONE;
  enum Frontend::Frontend_Index frontend = Frontend::TERM_SIMPLE;
  string text = "";

  bool use_text(void) {
    if (text.empty() == true) {
      Error::print("No code input specified.");
      return false;
    }
    return true;
  }

  void arguments(int argc, char* argv[]) {
    int opt = 0;
    while ((opt = getopt_long(argc, argv, opts, longopts, NULL)) != -1) {
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
          Verbose::print("Enable input from file");
          break;
        case 'i':
          if (optarg == NULL) {
            gstat = HELLO;
            Verbose::print("Set input to Hello World default");
            break;
          }
          gstat = TEXT;
          text = optarg;
          if (text == "-") {
            Verbose::print("Reading from stdin for input");
            std::cin >> text;
            break;
          }
          Verbose::print("Set argument as input");
          break;

        // FRONTEND
        case 'r':
          frontend = Frontend::NONE;
          Verbose::print("Enabled run mode");
          break;

        // OTHER
        case 'V':
          Verbose::verbose = true;
          Verbose::print("Enabled Verbose");
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
      Error::print("No code input specified.");
      Args::exec_help();
      return 0;

    case Args::FILE:
      if (Args::use_text() == false) return 0;
      if (!std::filesystem::exists(Args::text)) {
        Error::print("File does not exist.");
        return 0;
      }
      Verbose::print("Getting data from file");
      bf.get_file(Args::text);
      break;
    case Args::TEXT:
      if (Args::use_text() == false) return 0;

      Verbose::print("Getting data from input");
      bf.get(Args::text);
      break;
    case Args::HELLO:
      bf.get();
      break;
  }

  // Convert to IR
  Verbose::print("Converting to IR representation");
	bf.convert(Backend::C_IR);

	// Execute (Option for stepping through)
  Verbose::print("Running frontend");
	Frontend::frontend(bf, Args::frontend);
}
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
