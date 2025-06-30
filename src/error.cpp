#ifndef ERROR__HPP
#define ERROR__HPP

#include <sstream>
#include <cstring>
using namespace std;

// Error {{{
namespace Error {
	const char* Err_default = "An Error Occured";
	const char* Err_msg = "Error: ";

	void print(const string str) {
		int saved_errno = errno;

		cerr << C::R << Err_msg << C::RESET << str;

		if (str[str.length()-2] == ':')
			cerr << strerror(saved_errno);

		cerr << endl;
	}
	void print(void) {
		print(Err_default);
	}
	template< typename ... Args >
	void print(Args const& ... args) {
		ostringstream stream;
		using List = int[];
		(void)List{0, ( (void)(stream << args), 0) ... };

		print(stream.str());
	}


	void die(string str) {
		print(str);
		exit(1);
	}
	void die(void) {
		die(Err_default);
	}
	template< typename ... Args >
	void die(Args const& ... args) {
		ostringstream stream;
		using List = int[];
		(void)List{0, ( (void)(stream << args), 0) ... };

		die(stream.str());
	}
}
// }}}

// Verbose {{{
namespace Verbose {
  bool verbose = false;
  namespace {
    bool if_verbose(void) {
      return verbose;
    }
  }
#define IF_VERBOSE  if (if_verbose() == false) return;
#define VERBOSE_MSG "Verbose: "
  namespace {
    void h_print(string msg) {
      cerr << VERBOSE_MSG << msg << endl;
    }
  }
  void print(string msg) {
    IF_VERBOSE
    h_print(msg);
  }

	template< typename ... Args >
	void print(Args const& ... args) {
    IF_VERBOSE
		ostringstream stream;
		using List = int[];
		(void)List{0, ( (void)(stream << args), 0) ... };
		h_print(stream.str());
	}
#undef VERBOSE
}
// }}}

#endif
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
