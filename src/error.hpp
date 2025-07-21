#ifndef ERROR__HPP
#define ERROR__HPP

#include <sstream>
#include <cstring>

// TODO: merge with Verbose, into one log.hpp and Log namespace
// Error {{{
namespace Error {
	const std::string Err_default = "An Error Occured";
	const std::string Err_msg = "Error: ";

	void print(const std::string str) {
		int saved_errno = errno;

    std::cerr << C::R << Err_msg << C::RESET << str;

		if (str[str.length()-2] == ':')
			std::cerr << strerror(saved_errno);

    std::cerr << std::endl;
	}
	void print() {
		print(Err_default);
	}
	template< typename ... Args >
	void print(Args const& ... args) {
		std::ostringstream stream;
    ((stream << args), ...);

		print(stream.str());
	}


	void die(std::string str) {
		print(str);
		exit(1);
	}
	void die() {
		die(Err_default);
	}
	template< typename ... Args >
	void die(Args const& ... args) {
		std::ostringstream stream;
    ((stream << args), ...);

		die(stream.str());
	}
}
// }}}

// Verbose {{{
namespace Verbose {
  bool verbose = false;
  namespace {
    bool if_verbose() {
      return verbose;
    }
  }
#define IF_VERBOSE  if (if_verbose() == false) return;
#define VERBOSE_MSG "Verbose: "
  namespace {
    void h_print(std::string msg) {
      std::cerr << VERBOSE_MSG << msg << std::endl;
    }
  }
  void print(std::string msg) {
    IF_VERBOSE
    h_print(msg);
  }

	template< typename ... Args >
	void print(Args const& ... args) {
    IF_VERBOSE

		std::ostringstream stream;
    ((stream << args), ...);

		h_print(stream.str());
	}
#undef VERBOSE
}
// }}}

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
      o.fd << "[\033[" STR(M_RED) "mERROR:\033[0m ";

    ((o.fd << args), ...);

    o.fd << std::endl;
  }

}

#endif
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
