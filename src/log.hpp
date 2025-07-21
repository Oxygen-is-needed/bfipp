#ifndef LOG__HPP
#define LOG__HPP

// Log {{{
namespace Log {
#define LOG_METHODS       \
  X(NONE,     0)          \
  X(VM,       M_BLUE)     \
  X(BACKEND,  M_MAGENTA)  \
  X(FRONTEND, M_GREEN)    \
  X(SETTINGS, M_YELLOW)   \

  enum Log_Methods {
#define X(a,...) a,
    LOG_METHODS
#undef  X
  };

  const std::string_view log_methods[] {
#define X(a,b,...) "[\033[" STR(b) "m" STR(a) "\033[0m]: ",
    LOG_METHODS
#undef  X
  };
#undef  LOG_METHODS

  const unsigned int verbose_max = 1; ///< Max value that matters for verbose_level
  unsigned int verbose_level = 0; ///< Level at which to allow printing

  struct O {
    unsigned int v      = 0;
    bool e              = false;
    Log_Methods lm      = NONE;
    std::ostream& fd    = std::cout;
  };

  // Default Settings {{{
  struct O verbose = {
    .v = 1,
  };

  struct O error(Log_Methods lm = NONE, std::ostream& fd = std::cout) {
    return {
      .e = true,
      .lm = lm,
      .fd = fd
    };
  }
  // }}}


  template <typename... Args>
  void print(struct O o, Args const &...args) {
    if (o.v > verbose_level)
      return;

    if (o.lm != NONE)
      o.fd << log_methods[o.lm];
    if (o.e == true)
      o.fd << "\033[" STR(M_RED) "mERROR:\033[0m ";

    ((o.fd << args), ...);

    o.fd << std::endl;
  }

}
// }}}

#endif
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
