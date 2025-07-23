#ifndef CONFIG__HPP
#define CONFIG__HPP

// TODO: comment all setting options here

#define NT "\t\t\t\t"
#define NL "\n" NT

#define FRONTEND_DESCRIPTION                                                   \
  "Select a frontend to modify how the code is displayed during execution\n"    \
  "(or even how if it is viewed)."
#define FRONTEND_CONFIG                                                        \
  X(NONE, None::frontend, nullptr, "Just run and print program output.")       \
  X(TERM_SIMPLE, SimpleTextFrontend::frontend, SimpleTextFrontend::help,       \
    "A simple terminal frontend.")

#define BACKEND_CONFIG

#define PROGRAM_FLAGS                                                          \
  X('f', ':', ' ', "file", required_argument, "file: Input file.")             \
  X('h', ' ', ' ', "help", no_argument, "Prints helps text.")                  \
  X('H', ' ', ' ', "lhelp", no_argument, "Prints extra helps text.")           \
  X('i', ':', ':', "input", required_argument,                                 \
    "[txt]: Input text." NL "Default with no arguments." NL                    \
    "Use '-' for reading from stdin.")                                         \
  X('r', ' ', ' ', "run", no_argument,                                         \
    "Run input imediatly, using no frontend.")                                 \
  X('V', ' ', ' ', "verbose", no_argument, "Enable verbose output.")           \
  X('v', ' ', ' ', "version", no_argument, "Print version")                    \
  X('F', ':', ' ', "frontend", optional_argument,                              \
    "Change the frontend used to render." NL                                   \
    "Use no arguments to list frontends.")                                     \
  X('l', ' ', ' ', "list-fronts", no_argument, "List all available frontends.")

#define FRONTEND_DESCRIPTION__SIMPLE_TEXT \
  "Simple Text Frontend. A frontend using a terminal."
#define FRONTEND_KEYS__SIMPLE_TEXT                                             \
  X(QUIT, 'q', "Quit Frontend.")                                               \
  X(HELP, 'h', "Prints help text.")                                            \
  X(RUN,  'r', "Run entire program without user input.")                       \
  X(SKIP, 's', "num: Skip a defined amount each step.")                        \
  X(WAIT, 'w', "num: Wait till defined instruction counter.")

#define PRINT_LOG_METHODS                                                      \
  X(NONE,     0)                                                               \
  X(VM,       M_BLUE)                                                          \
  X(BACKEND,  M_MAGENTA)                                                       \
  X(FRONTEND, M_GREEN)                                                         \
  X(SETTINGS, M_YELLOW)


#endif
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
