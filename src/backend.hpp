#ifndef BACKEND__HPP
#define BACKEND__HPP

/* VM {{{ */
class VM {
  public:
    struct Rule {
      char cmd;
      std::vector<unsigned char> findex;
    };


    struct Rules {
      unsigned int tape_length = 30000; ///< Length of tape
      bool eot_wrap = true; ///< Does tape rap at end of tape
      std::vector<Rule> r;
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

    Rules rules;
    std::vector<unsigned char> instructions;
    unsigned int ins_max;
    unsigned int ins_i=0;
    unsigned int total_steps=0;
    std::string output;
    unsigned int pc=0;
  private:
    // TODO: make buffer dynamic at VM initalization.
#define BUF_LEN 30000
    unsigned char* buffer;
    std::stack<int> jump_list;

    /* Functions {{{ */
    void fnull() {}

    void f_inc() { buffer[pc]++; }
    void f_dec() { buffer[pc]--; }

    void f_right() {
      if (pc >= rules.tape_length) {
        if (rules.eot_wrap == true) {
          pc = 0;
          return;
        }
        pc = rules.tape_length;
        return;
      }
      pc++;
    }
    void f_left() {
      if (pc <= 0) {
        if (rules.eot_wrap == true) {
          pc = rules.tape_length;
          return;
        }
        pc = 0;
        return;
      }

      pc--;
    }

    void f_print() { output += buffer[pc]; }
    void f_input() { buffer[pc] = getchar(); }

    void f_push() {
      if (!jump_list.empty()) {
        if (static_cast<unsigned int>(jump_list.top()) == ins_i)
          return;
      }
      jump_list.push(ins_i);
    }
    void f_pop() { if (!jump_list.empty()) jump_list.pop(); }

    // TODO: remove need of this function.
    void f_seek() {
      if (buffer[pc] != 0) return;
      int f=0;
      for (unsigned int x=ins_i+1; x<ins_max; x++) {
        if (rules.r[instructions[x]].cmd == ']') {
          if (f==0) {
            ins_i=x;
            return;
          }
          f--;
        }
        if (rules.r[instructions[x]].cmd == '[')
          f++;
      }
    }

    // TODO: remove need of this function.
    void f_seekb() {
      if (buffer[pc] == 0) return;
      int f=0;
      for (ssize_t x=ins_i-1; x>=0; x--) {
        if (rules.r[instructions[x]].cmd == '[') {
          if (f==0) {
            ins_i=x;
            return;
          }
          f--;
        }
        if (rules.r[instructions[x]].cmd == ']')
          f++;
      }
    }

    void f_jz() {
      if (jump_list.empty()) return;
      int a = jump_list.top();
      if (a > 0) a--;
      if (buffer[pc] == 0) ins_i = a;
    }
    void f_jnz() {
      if (jump_list.empty()) return;
      int a = jump_list.top();
      if (a > 0) a--;
      if (buffer[pc] != 0) ins_i = a;
    }

    using FPtrs = void (VM::*)();
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
    VM(Rules r, std::vector<unsigned char> i)
      : buffer(new unsigned char[r.tape_length] {0}) {
        rules.tape_length = r.tape_length;
        rules.tape_length = r.tape_length;
        rules.r.assign(r.r.begin(), r.r.end());

        instructions.assign(i.begin(), i.end());
        ins_max = instructions.size();
      }
    ~VM() {}

    bool step() {
      total_steps++;
      if (ins_i >= ins_max)
        return false;

      run_funcs(rules.r[instructions[ins_i]]);
      ins_i++;
      return true;
    }
    void run() {
      while(step() == true);
    }


    // TODO: should move to frontend?
    unsigned int inspect_buffer(ssize_t mwidth=100, int view_frame=14) {
      if (mwidth == -1) {
        mwidth = std::numeric_limits<int>::max();
      }

      int width=view_frame;
      unsigned int out=5;
      std::cout << total_steps << ": ";
      unsigned int y=0;
      for (int x=0; x<width && y<mwidth && y<rules.tape_length; x++,y++) {
        if (buffer[y] != 0) {
          x=0;
        }

        std::string code = "";
        if (buffer[y] != 0) {
          if (isprint(buffer[y]) == 0 || buffer[y] == ' ') {
            code = std::to_string(buffer[y]);
          } else  {
            code += buffer[y];
          }
        }

        if (pc == y) {
          std::cout << C::B << "{" << C::Y
            << code
            << C::B << "} " << C::RESET;
          continue;
        }

        std::cout << "[" << code << "] ";
      }
      std::cout << std::endl;
      out += 4*y;
      return out;
    }
};
/* }}} */

/* Backend {{{ */
class Backend {
  // TODO: remove need for macro
#define STATE(s)                                                               \
  if (state != s) {                                                            \
    Log::print(error, "Use of function '", __FUNCTION__,                       \
        "' in invalid state.");                                         \
    exit(1);                                                                   \
  }

  Log::O error = {
    .v  = 0,
    .e  = true,
    .lm = Log::BACKEND,
  };

  public:
  enum State {
    SET=0,	///< SET: Settings
    GET=1,	///< GET: Get data
    CON=2,	///< Convert IR: Convert to IR
    EXE=3,	///< EXEcute: EXEcute code and or EXport.
  } state=SET;

  /* SET {{{ */
  private:
  VM::Rules rules;
  public:
  Backend () {
    STATE(SET);

    rules.tape_length = 30000;
    rules.eot_wrap = true;
    rules.r.push_back({'+', {VM::INC}});
    rules.r.push_back({'-', {VM::DEC}});
    rules.r.push_back({'>', {VM::RIGHT}});
    rules.r.push_back({'<', {VM::LEFT}});
    rules.r.push_back({'[', {VM::SEEK}});
    rules.r.push_back({']', {VM::SEEKB}});
    rules.r.push_back({'.', {VM::PRINT}});
    rules.r.push_back({',', {VM::INPUT}});

    state = GET;
  }

  // TODO: add to arguments
  void print_rules() {
    if (state <= SET) return;
    if (rules.r.size() <= 0) {
      Log::print(error,"Invalid rule set");
    }

    std::cout << "(CMD, Actions)" << std::endl;
    for (auto r : rules.r) {
      std::cout << " " << r.cmd << " = {";
      int len = r.findex.size()-1;
      if (len >= 1) {
        for (; len>0; len--) {
          std::cout << r.findex[len] << ",";
        }
      }
      std::cout << r.findex[len] << "}" << std::endl;
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
      for (auto r : rules.r) {
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
  void get() {
    // "Hello, world!\n" in bf
    get(">+++++++++[<++++++++>-]<.>+++++++[<++++>-]<+.+++++++..+++.[-]>++"
        "++++++[<++++>-]<.>+++++++++++[<++++++++>-]<-.--------.+++.------"
        ".--------.[-]>++++++++[<++++>-]<+.[-]++++++++++.");
  }
  void get_file(std::filesystem::path filename) {
    STATE(GET);

    // Open File
    if (!std::filesystem::exists(filename)) {
      Log::print(error, "File '", filename, "' does not exist");
      return;
    }
    std::ifstream fd(filename);
    if (!fd.is_open()) {
      Log::print(error, "File Descriptor for '", filename, "' was not openend");
      exit(1);
    }

    // Sanitize and Append code
    std::string line;
    while (std::getline(fd, line) && state == GET) {
      code += sanitize_input(line);
    }

    fd.close();

    if (code.empty()) {
      Log::print(error, "File empty or full of unrealted charactors.");
      exit(1);
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
      for (unsigned int x=0; x<(rules.r.size()); x++) {
        if (rules.r[x].cmd == c) {
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
    if (code.empty()) {
      Log::print(error, "Unable to interpret nothing. Code string is empty.");
      exit(1);
    }

    convert_string2ir(code);

    state = EXE;
  }
  /* }}} */

  /* EXE {{{ */
  private:
  bool vm_using = false;
  std::vector<VM> vm_list;

  bool ir_vm_step() {
    if (vm_using != true || vm_list.empty()) {
      Log::print(error, "VM has not been initalized!");
      exit(1);
    }

    return vm_list[0].step();
  }
  bool ir_vm_setup() {
    if (rules.r.empty() || instructions.empty()) {
      Log::print(error, "Invalid Rules and/or Instructions setup.");
      exit(1);
    }

    if (!vm_list.empty()) {
      Log::print(error, "TODO: cannot have multiple VM's at once.");
      exit(1);
    }

    VM vm(rules, instructions);
    vm_list.push_back(vm);
    vm_using = true;

    return true;
  }
  void ir_vm() {
    ir_vm_setup();
    if (vm_using != true || vm_list.empty()) {
      Log::print(error, "VM has not been initalized!");
      exit(1);
    }
  }

  struct {
    bool (Backend::*step)();
    bool (Backend::*init)();
    void (Backend::*use)();
  } Exec_Funcs[1] = {
    [C_IR] = { &Backend::ir_vm_step, &Backend::ir_vm_setup, &Backend::ir_vm}
  };

  public:
  void execute_set() {
    if ((this->*Exec_Funcs[0].init)() != true) {
      Log::print(error, "Invalid Initalization");
      exit(1);
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
  VM& get_vm() {
    if (vm_list.empty()) {
      Log::print(error, "Cannot get a VM that is not initalized.");
      exit(1);
    }
    VM& vm = vm_list[0];
    return vm;
  }
  /* }}} */
};
/* }}} */

#endif
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
