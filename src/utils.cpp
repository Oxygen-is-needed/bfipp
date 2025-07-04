#ifndef UTILS__HPP
#define UTILS__HPP
#include <signal.h>
#include <cstring>

#define _STR(x)	#x
#define STR(x)	_STR(x)

// Colors {{{
namespace C {
	enum Colors {
	RESET		= 0,
	BLACK		= 30, Z = 30,
	RED     	= 31, R = 31,
	GREEN   	= 32, G = 32,
	YELLOW  	= 33, Y = 33,
	BLUE    	= 34, B = 34,
	MAGENTA 	= 35, M = 35,
	CYAN    	= 36, C = 36,
	WHITE   	= 37, W = 37,

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
		os << "\033[" << (int)a << "m";
		return os;
	}
}
// }}}
// Unwinding {{{
namespace Unwind {
  namespace {
    struct Unwind {
      void (*fptr)(void);
      std::string name;
    };
    std::vector<Unwind> unwind_vector;
    bool unwind_b=false;
  }

  void pop_unwind(void) {
    unwind_vector.erase(--unwind_vector.end());
  }
  namespace {
    void run_unwind(void) {
      if (unwind_vector.empty() == true || unwind_b == true) {
        return;
      }
      for (int x=unwind_vector.size()-1; x>=0; x--) {
        unwind_vector[x].fptr();
#ifdef DEBUG
        std::cerr << C::M << "UNWIND: " << C::RESET << unwind_vector[x].name << endl;
#endif
        pop_unwind();
      }
    }
  }

  void add_unwind(struct Unwind u) {
    unwind_vector.push_back(u);
  }
  void unwind(void) {
    run_unwind();
    exit(0);
  }

  namespace {
    void handle_sigint(int) {
#ifdef DEBUG
      std::cerr << "Signal Caught: " << sig << endl;
#endif
      run_unwind();
      exit(0);
    }
  }
  void signal_handler(void) {
#ifdef _WIN32
    signal(SIGINT,  handle_sigint);
    signal(SIGTERM, handle_sigint);
    signal(SIGABRT, handle_sigint);
#else
    struct sigaction sa;

    sa.sa_handler   = &handle_sigint;
    sa.sa_flags     = SA_RESTART;

    sigaction(SIGINT,   &sa, NULL);
    sigaction(SIGTERM,  &sa, NULL);
    sigaction(SIGABRT,  &sa, NULL);
#endif
  }

  void on_exit(void) {
    if (unwind_b == true)
      return;
    run_unwind();
  }
  void initalize_unwind(void) {
    signal_handler();
    atexit(on_exit);
  }
}
// }}}

#endif
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
