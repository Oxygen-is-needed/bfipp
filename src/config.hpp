#ifndef CONFIG__HPP
#define CONFIG__HPP

/**
 * define NT - New Tabs for multi line key descriptions.
 */
#define NT "\t\t\t\t"
/**
 * define NL - New Line for multi line key descriptions.
 *
 * Uses NT to specify where all newlines should be in comparison for key
 * descriptor macros.
 */
#define NL "\n" NT


/**
 * define PROGRAM_DESCRIPTION - Program Description.
 */
#define PROGRAM_DESCRIPTION                                                    \
  "`bfi++` is a Brainf*** interpreter and virtual machine designed for "       \
  "\nadaptability and extensibility, offering multiple frontends to visualize "\
  "\nmemory and code execution."

/**
 * define PROGRAM_FLAGS - Program CLI flags.
 *
 * context:
 *  There are three argument types:
 *    - no_argument
 *    - optional_argument
 *    - required_argument
 *
 * context: the two arguments after the single flag are used to determine if the
 *  flag has no arguments, or required arguments (':', ' '), or optional
 *  arguments (':', ':').
 *
 * context: if the long flag is changed you will have to change the getopt
 *  implementation names along with it.
 *
 *
 * Used to configure the arguments used when executing the program.
 * There are 6 arguments to the macro X:
 *    1) single flag name
 *    2) single flag argument type
 *    3) single flag argument type
 *    4) long flag name.
 *      - should be without quotes
 *    5) long flag argument type
 *    6) argument description
 */
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
  X('l', ' ', ' ', "list-fronts", no_argument,                                 \
    "List all available frontends.")                                           \
  X('F', ':', ' ', "frontend", optional_argument,                              \
    "Change the frontend used to render." NL                                   \
    "Use no arguments to list frontends.")


/**
 * define FRONTEND_DESCRIPTION - Description for what frontends are.
 */
#define FRONTEND_DESCRIPTION                                                   \
  "Select a frontend to modify how the code is displayed during execution\n"   \
  "(or even how if it is viewed)."
/**
 * define FRONTEND_CONFIG - Configuration for Frontends.
 *
 * context: Fields 3, frontend help print function can be ignored if `nullptr`
 *  is placed there instead.
 *
 * context: Field 4, frontend description, can be ignore if replaced with `""`
 *  (or an empty string).
 *
 * Defines the parts needed to use a frontend. They break down into four
 * arguments in the macro X:
 *  1) Name and enum of the Frontend.
 *  2) Frontend macro, used to execute the frontend.
 *  3) Frontend help macro, used to print the keybindings.
 *  4) Frontend description.
 */
#define FRONTEND_CONFIG                                                        \
  X(NONE, None::frontend, nullptr, "Just run and print program output.")       \
  X(SIMPLE_TEXT, SimpleTextFrontend::frontend, SimpleTextFrontend::help,       \
    "A simple terminal frontend.")


/**
 * define FRONTEND_DESCRIPTION__SIMPLE_TEXT - Description for SIMPLE_TEXT
 *  frontend.
 */
#define FRONTEND_DESCRIPTION__SIMPLE_TEXT \
  "Simple Text Frontend. A frontend using a terminal."
/**
 * define FRONTEND_KEYS__SIMPLE_TEXT - Keys for SIMPLE_TEXT frontend.
 *
 * The keys utilized for SIMPLE_TEXT, along with their enum names and descriptions.
 * They are divided into three arguments within macro X:
 *  1) Enum and name of the key.
 *  2) Key employed by SIMPLE_TEXT.
 *  3) Description of the keybind usage.
 */
#define FRONTEND_KEYS__SIMPLE_TEXT                                             \
  X(QUIT, 'q', "Quit Frontend.")                                               \
  X(HELP, 'h', "Prints help text.")                                            \
  X(RUN,  'r', "Run entire program without user input.")                       \
  X(SKIP, 's', "num: Skip a defined amount each step.")                        \
  X(WAIT, 'w', "num: Wait till defined instruction counter.")

/**
 * define PRINT_LOG_METHODS - Names and Colors used for Logging each method.
 *
 * context: NONE is a special case that will not print itself.
 *
 * The Names and colors used when logging each method, specified when invoking
 * the `Log::print()` function.
 * They are divided into two arguments within macro X:
 *  1) Log name, used to invoke the method.
 *  2) Log colro, used to color the name when printing.
 */
#define PRINT_LOG_METHODS                                                      \
  X(NONE,     0)                                                               \
  X(VM,       M_BLUE)                                                          \
  X(BACKEND,  M_MAGENTA)                                                       \
  X(FRONTEND, M_GREEN)                                                         \
  X(SETTINGS, M_YELLOW)


#endif
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
