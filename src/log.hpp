#ifndef LOG__HPP
#define LOG__HPP

// Log {{{
namespace Log {
#define LOG_METHODS PRINT_LOG_METHODS

#define X(A, ...) 1,
#define COUNT(...) (sizeof((int[]){__VA_ARGS__}) / sizeof(int))
  const int __frontend_config_len = COUNT(LOG_METHODS);
#define LM_LEN
#undef COUNT
#undef X

  enum Log_Methods {
#define X(a, ...) a,
    LOG_METHODS
#undef X
  };

  std::string_view log_methods_no_color[LM_LEN] = {
#define X(a, b, ...) STR(a),
      LOG_METHODS
#undef X
  };

  std::string_view log_methods_color[LM_LEN] = {
#define X(a, b, ...) "[\033[" STR(b) "m" STR(a) "\033[0m]: ",
      LOG_METHODS
#undef X
  };

  std::string_view (*log_methods_list[2])[LM_LEN] = {
      &log_methods_no_color,
      &log_methods_color,
  };
  std::string_view (*log_methods)[LM_LEN] = log_methods_list[1];

#undef LM_LEN
#undef LOG_METHODS

  const unsigned verbose_max = 2;  ///< Max value that matters for verbose_level
  unsigned int verbose_level = 0;  ///< Level at which to allow printing

  struct O {
    unsigned int v = 0;            ///< What level of verbose is log request
    bool e = false;                ///< Is log request an error
    Log_Methods lm = NONE;         ///< What is the name of the request
    std::ostream& fd = std::cout;  ///< What stream to log to
  };

  std::string_view error_colors[2] = {"", "\033[" STR(M_RED) "m"};
  std::string_view& error_color = error_colors[1];

  // Default Settings {{{
  struct O verbose = {
      .v = 1,
  };

  struct O error(Log_Methods lm = NONE, std::ostream& fd = std::cout) {
    return {
        .e = true,
        .lm = lm,
        .fd = fd,
    };
  }
  // }}}

  template <typename... Args>
  void print(const struct O& o, Args const&... args) {
    if (o.v > verbose_level)
      return;

    if (o.lm != NONE)
      o.fd << (*log_methods)[o.lm];
    if (o.e == true)
      o.fd << error_color << "ERROR:\033[0m ";

    ((o.fd << args), ...);

    o.fd << std::endl;
  }

  void color(bool enable) {
    error_color = error_colors[enable];
    log_methods = log_methods_list[enable];
  }

}
// }}}

#endif
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
