#ifndef UTILS__HPP
#define UTILS__HPP
#include <signal.h>
#include <cstring>

#define _STR(x)	#x
#define STR(x)	_STR(x)

// Colors {{{
namespace C {
#define M_RESET		  0
#define M_BLACK		  30
#define M_RED     	31
#define M_GREEN   	32
#define M_YELLOW  	33
#define M_BLUE    	34
#define M_MAGENTA 	35
#define M_CYAN    	36
#define M_WHITE   	37

	enum Colors {
	RESET		  = M_RESET,
	BLACK		  = M_BLACK,    Z = M_BLACK,
	RED     	= M_RED,      R = M_RED,
	GREEN   	= M_GREEN,    G = M_GREEN,
	YELLOW  	= M_YELLOW,   Y = M_YELLOW,
	BLUE    	= M_BLUE,     B = M_BLUE,
	MAGENTA 	= M_MAGENTA,  M = M_MAGENTA,
	CYAN    	= M_CYAN,     C = M_CYAN,
	WHITE   	= M_WHITE,    W = M_WHITE,

	BOLD    	= 1,
	DIM     	= 2,
	ITALIC  	= 3,
	UNDERLINE	= 4,
	INVERSE 	= 7,
	};


	void set_color(int a) {
    std::cout << "\033[" << a << "m";
	}

	void clear(std::ostream& stream=std::cout) {
		stream << "\033[2J\033[H";
	}
	void clear_c2end(std::ostream& stream=std::cout) {
		stream << "\033[0J";
	}

	void hide_cursor(std::ostream& stream=std::cout) {
		stream << "\033[?25l";
	}
	void show_cursor(std::ostream& stream=std::cout) {
		stream << "\033[?25h";
	}

	void move_up(unsigned int i=1, std::ostream& stream=std::cout) {
		stream << "\033[" << i << "A";
	}
	void move_down(unsigned int i=1, std::ostream& stream=std::cout) {
		stream << "\033[" << i << "B";
	}

	std::ostream& reset(std::ostream& os) {
		os << "\033[0;0m";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, Colors a) {
		os << "\033[" << static_cast<int>(a) << "m";
		return os;
	}
}
// }}}

#endif
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
