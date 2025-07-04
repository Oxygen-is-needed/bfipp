#include <stack>
#include <filesystem>
#include <fstream>
#include <limits>
#include "utils.cpp"
#include "error.cpp"


/* VM {{{ */
class VM {
public:
	struct Rule {
		char cmd;
		vector<unsigned char> findex;
	};
	enum Funcs {
		INC,    ///< INCrement data at program pointer
		DEC,    ///< DECrement data at program pointer
		RIGHT,  ///< move program pointer RIGHT
		LEFT,   ///< move program pointer LEFT
		PRINT,  ///< PRINT: put data at current pointer to screen
		INPUT,  ///< INPUT: get input
		PUSH,	  ///< PUSH: push PC to stack
		POP,	  ///< POP: pop top PC value from stack and use it
		JZ,	    ///< Jump to top of stack if Zero
		JNZ,	  ///< Jump to top of stack if Not-Zero
		SEEK,   ///< SEEK forward to a bracket
		SEEKB,  ///< SEEK Backwards to a bracket
		_FUNCS_LEN
	};

	std::vector<Rule> rules;
	std::vector<unsigned char> instructions;
	unsigned int ins_max;
	unsigned int ins_i=0;
	unsigned int total_steps=0;
	string output;
	unsigned int pc=0;
private:
	// TODO: make buffer dynamic at VM initalization.
#define BUF_LEN 30000
	unsigned char buffer[BUF_LEN] = {0};
	stack<int> jump_list;

	/* Functions {{{ */
	void fnull(void) {}

	void f_inc(void) { buffer[pc]++; }
	void f_dec(void) { buffer[pc]--; }

	void f_right(void) { pc++; pc%=BUF_LEN; }
	void f_left(void) { pc--; pc%=BUF_LEN; }

	void f_print(void) {
#ifdef DEBUG
		std::cout << "'" << buffer[pc] << "'";
#endif
		output += buffer[pc];
	}
	void f_input(void) { buffer[pc] = getchar(); }

	void f_push(void) {
		if (!jump_list.empty()) {
			if ((unsigned int)jump_list.top() == ins_i) return;
    }
		jump_list.push(ins_i);
	}
	void f_pop(void) { if (!jump_list.empty()) jump_list.pop(); }

  // TODO: remove need of this function.
	void f_seek(void) {
		if (buffer[pc] != 0) return;
		int f=0;
		for (unsigned int x=ins_i+1; x<ins_max; x++) {
			if (rules[instructions[x]].cmd == ']') {
				if (f==0) {
					ins_i=x;
					return;
				}
				f--;
			}
			if (rules[instructions[x]].cmd == '[')
				f++;
		}
	}

  // TODO: remove need of this function.
	void f_seekb(void) {
		if (buffer[pc] == 0) return;
		int f=0;
		for (ssize_t x=ins_i-1; x>=0; x--) {
			if (rules[instructions[x]].cmd == '[') {
				if (f==0) {
					ins_i=x;
					return;
				}
				f--;
			}
			if (rules[instructions[x]].cmd == ']')
				f++;
		}
	}

	void f_jz(void) {
		if (jump_list.empty()) return;
		int a = jump_list.top();
		if (a > 0) a--;
		if (buffer[pc] == 0) ins_i = a;
	}
	void f_jnz(void) {
		if (jump_list.empty()) return;
		int a = jump_list.top();
		if (a > 0) a--;
		if (buffer[pc] != 0) ins_i = a;
	}

	using FPtrs = void (VM::*)(void);
	FPtrs funcs[_FUNCS_LEN] = {
		[INC]	  = &VM::f_inc,
		[DEC]	  = &VM::f_dec,
		[RIGHT]	= &VM::f_right,
		[LEFT]	= &VM::f_left,
		[PRINT]	= &VM::f_print,
		[INPUT]	= &VM::f_input,
		[PUSH]	= &VM::f_push,
		[POP]	  = &VM::f_pop,
		[JZ]	  = &VM::f_jz,
		[JNZ]	  = &VM::f_jnz,
		[SEEK]  = &VM::f_seek,
		[SEEKB] = &VM::f_seekb,
	};

	/* }}} */

	void run_funcs(Rule& r) {

		for (auto i : r.findex) {
      if (i < _FUNCS_LEN) {
        (this->*funcs[i])();
        continue;
      }
		}
	}
public:

	VM(std::vector<Rule> r, std::vector<unsigned char> i) {
		rules.assign(r.begin(), r.end());
		instructions.assign(i.begin(), i.end());
		ins_max = instructions.size();
	}

	bool step(void) {
		total_steps++;
		if (ins_i >= ins_max)
			return false;

		run_funcs(rules[instructions[ins_i]]);
    ins_i++;
		return true;
	}
	void run(void) {
		while(step() == true);
	}


	unsigned int inspect_buffer(ssize_t mwidth=100, int view_frame=14) {
		if (mwidth == -1) {
      mwidth = std::numeric_limits<int>::max();
    }

		int width=view_frame;
		unsigned int out=5;
		std::cout << total_steps << ": ";
		unsigned int y=0;
		for (int x=0; x<width && y<mwidth; x++,y++) {
			if (buffer[y] != 0) {
				x=0;
			}
			if (pc == y) {
				std::cout << C::B << "{" << C::Y
					<< (unsigned int) buffer[y]
					<< C::B << "} " << C::RESET;
				continue;
			}
			std::cout << "[" << (unsigned int) buffer[y] << "] ";
		}
		std::cout << endl;
    out += 4*y;
    return out;
	}
};
/* }}} */

/* Backend {{{ */
class Backend {
#define STATE(s)	if (state != s) { Error::die("Use of function '", __FUNCTION__, "' in invalid state."); }
public:
	enum State {
		SET=0,	///< SET: Settings
		GET=1,	///< GET: Get data
		CON=2,	///< Convert IR: Convert to IR
		EXE=3,	///< EXEcute: EXEcute code and or EXport.
	} state=SET;

	/* SET {{{ */
private:
	std::vector<VM::Rule> rules;
public:
	Backend (void) {
		STATE(SET);

		rules.push_back({'+', {VM::INC}			});
		rules.push_back({'-', {VM::DEC}			});
		rules.push_back({'>', {VM::RIGHT}		});
		rules.push_back({'<', {VM::LEFT}		});
		rules.push_back({'[', {VM::SEEK}	  });
		rules.push_back({']', {VM::SEEKB}	  });
		rules.push_back({'.', {VM::PRINT}		});
		rules.push_back({',', {VM::INPUT}		});

		state = GET;
	}

  // TODO: add to arguments
	void print_rules(void) {
		if (state <= SET) return;
    if (rules.size() <= 0)
      Error::die("Invalid rule set");

		std::cout << "(CMD, Actions)" << endl;
		for (auto r : rules) {
			std::cout << " " << r.cmd << " = {";
			int len = r.findex.size()-1;
			if (len >= 1) {
				for (; len>0; len--) {
					std::cout << (unsigned int)r.findex[len] << ",";
				}
			}
			std::cout << (unsigned int)r.findex[len] << "}" << endl;
		}
	}
	/* }}} */

	/* GET {{{ */
	std::string code;
private:
  bool is_end(char c) {
    switch(c) {
      case 0x04:  // (EOT) End Of Transmission
      case 0x03:  // (ETX) End of TeXt
      case 0x1a:  // (Sub) Substitute
      case 0x00:
        return true;
    }
    return false;
  }
	std::string sanitize_input(std::string s) {
		std::string out;
		for (auto c : s) {
			for (auto r : rules) {
        if (is_end(c) == true) {
          state = CON;
          return out;
        }
				if (c == r.cmd) {
					out += c;
				}
			}
		}

		return out;
	}
public:
	void get(std::string s) {
		STATE(GET);

		code = sanitize_input(s);

		state = CON;
	}
	void get(void) {
		// "Hello, world!\n" in bf
		get(">+++++++++[<++++++++>-]<.>+++++++[<++++>-]<+.+++++++..+++.[-]>++"
			"++++++[<++++>-]<.>+++++++++++[<++++++++>-]<-.--------.+++.------"
			".--------.[-]>++++++++[<++++>-]<+.[-]++++++++++.");
        }
	void get_file(std::filesystem::path filename) {
		STATE(GET);

		// Open File
		if (!std::filesystem::exists(filename)) {
			Error::print("File '", filename, "' does not exist");
			return;
		}
		ifstream fd(filename);
		if (!fd.is_open()) {
			Error::die("File Descriptor for '", filename, "' was not openend");
		}

		// Sanitize and Append code
		std::string line;
		while (std::getline(fd, line) && state == GET) {
			code += sanitize_input(line);
		}

		fd.close();

		if (code.empty()) {
			Error::die("File empty or full of unrealted charactors.");
		}

		state = CON;
	}
	/* }}} */

	/* CON {{{ */
	enum Conversion {
		C_NONE	= -1,
		C_IR	=  0,
	};
	enum Conversion conversion_method = C_NONE;
private:
	std::vector<unsigned char> instructions;
	void convert_string2ir(std::string s) {
		for (char c : s) {
			for (unsigned int x=0; x<(rules.size()); x++) {
				if (rules[x].cmd == c) {
					instructions.push_back(x);
					break;
				}
			}
		}
	}
public:

	// TODO: other methods of conversion from IR.
	void convert(enum Conversion con=C_IR /*,std::string output=""*/) {
		STATE(CON);

    // TODO: add file out
		//if (!output.empty()) {}
		conversion_method = con;
		if (code.empty())
			Error::die("Unable to interpret nothing. Code string is empty.");
        	convert_string2ir(code);

		state = EXE;
	}
	/* }}} */

	/* EXE {{{ */
private:
	bool vm_using = false;
	std::vector<VM> vm_list;

	bool ir_vm_step(void) {
		if (vm_using != true || vm_list.empty()) { Error::die("VM has not been initalized!"); }

		return vm_list[0].step();
	}
	bool ir_vm_setup(void) {
		if (rules.empty() || instructions.empty()) { Error::die("Invalid Rules and/or Instructions setup."); } 

		if (!vm_list.empty()) Error::die("TODO: cannot have multiple VM's at once.");

		VM vm(rules, instructions);
		vm_list.push_back(vm);
		vm_using = true;

		return true;
	}
	void ir_vm(void) {
		ir_vm_setup();
		if (vm_using != true || vm_list.empty()) { Error::die("VM has not been initalized!"); }
	}

	struct {
		bool (Backend::*step)(void);
		bool (Backend::*init)(void);
		void (Backend::*use)(void);
	} Exec_Funcs[1] = {
		[C_IR] = { &Backend::ir_vm_step, &Backend::ir_vm_setup, &Backend:: ir_vm}
	};
public:
	void execute_set() {
		if ((this->*Exec_Funcs[0].init)() != true) {
      Error::die("Invalid Initalization");
    }
	}
	bool execute_step() {
		return (this->*Exec_Funcs[0].step)();
	}
	void execute(/*Conversion em=C_IR*/) {
		STATE(EXE);

		execute_set();
		(this->*Exec_Funcs[0].use)();
	}
	VM& get_vm(void) {
		if (vm_list.empty()) Error::die("Cannot get a VM that is not initalized.");
		VM& vm = vm_list[0];
		return vm;
	}
	/* }}} */
};
/* }}} */

// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
