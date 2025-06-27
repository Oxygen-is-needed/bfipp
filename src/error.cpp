#ifndef ERROR__HPP
#define ERROR__HPP

#include <sstream>
#include <cstring>
using namespace std;

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

#endif
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
