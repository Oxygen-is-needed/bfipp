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
// Unwinding {{{
namespace Unwind {
  namespace {
    struct Unwind {
      void (*fptr)();
      std::string name;
    };
    std::vector<Unwind> unwind_vector;
    bool unwind_b=false;
  }

  void pop_unwind() {
    unwind_vector.erase(--unwind_vector.end());
  }
  namespace {
    void run_unwind() {
      if (unwind_vector.empty() == true || unwind_b == true) {
        return;
      }
      for (int x=unwind_vector.size()-1; x>=0; x--) {
        unwind_vector[x].fptr();
        pop_unwind();
      }
    }
  }

  void add_unwind(struct Unwind u) {
    unwind_vector.push_back(u);
  }
  void unwind() {
    run_unwind();
    exit(0);
  }

  namespace {
    void handle_sigint(int) {
      run_unwind();
      exit(0);
    }
  }
  void signal_handler() {
#ifdef _WIN32
    signal(SIGINT,  handle_sigint);
    signal(SIGTERM, handle_sigint);
    signal(SIGABRT, handle_sigint);
#else
    struct sigaction sa;

    sa.sa_handler   = &handle_sigint;
    sa.sa_flags     = SA_RESTART;

    sigaction(SIGINT,   &sa, nullptr);
    sigaction(SIGTERM,  &sa, nullptr);
    sigaction(SIGABRT,  &sa, nullptr);
#endif
  }

  void on_exit() {
    if (unwind_b == true)
      return;
    run_unwind();
  }
  void initalize_unwind() {
    signal_handler();
    atexit(on_exit);
  }
}
// }}}

#endif
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
