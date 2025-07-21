#ifndef CONFIG__HPP
#define CONFIG__HPP

// TODO: comment all setting options here

#define PRINT_LOG_METHODS                                                      \
  X(NONE,     0)                                                               \
  X(VM,       M_BLUE)                                                          \
  X(BACKEND,  M_MAGENTA)                                                       \
  X(FRONTEND, M_GREEN)                                                         \
  X(SETTINGS, M_YELLOW)

#define FRONTEND_KEYS__SIMPLE_TEXT                                             \
  X(QUIT, 'q', "Quit Frontend.")                                               \
  X(HELP, 'h', "Prints help text.")                                            \
  X(RUN,  'r', "Run entire program without user input.")                       \
  X(SKIP, 's', "num: Skip a defined amount each step.")                        \
  X(WAIT, 'w', "num: Wait till defined instruction counter.")

#define PROGRAM_FLAGS                                                          \
  X('f', ':', ' ', "file",      required_argument,  "file: Input file.")       \
  X('h', ' ', ' ', "help",      no_argument,        "Prints helps text.")      \
  X('H', ' ', ' ', "long-help", no_argument,        "Prints helps text.")      \
  X('i', ':', ':', "input",     required_argument,  "[txt]: Input text.\n\t\tDefault with no arguments.\n\t\t'-' for reading from stdin.") \
  X('r', ' ', ' ', "run",       no_argument,        "Run input imediatly.")    \
  X('V', ' ', ' ', "verbose",   no_argument,        "Enable verbose output.")  \
  X('v', ' ', ' ', "version",   no_argument,        "Print version")

#define FRONTEND_CONFIG \
  X(NONE,         None::frontend)  \
  X(TERM_SIMPLE,  Simple_Text_Frontend::frontend)

#endif
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
