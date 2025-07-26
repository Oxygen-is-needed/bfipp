#ifndef OUTPUT__HPP
#define OUTPUT__HPP

// Output {{{
namespace Output {
  Log::O error = {
    .e = true,
    .lm = Log::OUTPUT,
  };
  Log::O verbose = {
    .v = 1,
    .lm = Log::OUTPUT,
  };

  struct Context {
    struct VM::Import i;
    std::ofstream& fp;
    enum Frontend::Frontend_Index fi = Frontend::NONE;
    std::filesystem::path& fn;
  };
}
// }}}

// Output Utilities {{{
namespace OutputUtils {
  void set_executable(std::filesystem::path& str) {
    std::filesystem::permissions(str,
                                 std::filesystem::perms::group_all |
                                     std::filesystem::perms::owner_all,
                                 std::filesystem::perm_options::add);
  }
}
// }}}
// Output Methods {{{
namespace RawOut {
  void output(Output::Context c) {
    c.fp << "#!/bin/env -S bfi++";
    if (c.fi != Frontend::NONE) {
      c.fp << " -F" << static_cast<int>(c.fi)+1;
    }
    c.fp << " -f\n";

    c.fp << c.i.c << '\n';
    OutputUtils::set_executable(c.fn);
  }
};
// }}}

// Output {{{
namespace Output {
#define METHODS                                                                \
  OUTPUT_CONFIG                                                                \
  X(_LEN, nullptr)                                                             \
  X(__NONE__, nullptr)

  enum Method {
#define X(A,B,...)	A,
    METHODS
#undef	X
  };

  struct Functions {
    const std::string_view name;
    void (*const func)(struct Context);
  };
  const struct Functions functions[] = {
#define X(A,B,...)  { .name = STR(A) , .func = B },
    METHODS
#undef  X
  };

#undef	METHODS

  void output(Backend &bf, Frontend::Frontend_Index frontend,
              std::filesystem::path filename, enum Output::Method m) {

    if (m == Output::__NONE__) {
      Log::print(error, "Invalid Output method given.");
      exit(1);
    }

    if (filename.empty()) {
      Log::print(error, "Improper file path given.");
      return;
    }

    std::ofstream file(filename);
    Log::print(verbose, "Outputing file as ", functions[m].name, " to the file ",
               filename);
    functions[m].func({
        .i = bf.export_vm(),
        .fp = file,
        .fi = frontend,
        .fn = filename
    });
    file.close();
  }
}
// }}}

#endif
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
