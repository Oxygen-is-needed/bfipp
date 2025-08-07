#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <limits>
#include <print>
#include <stack>
#include <string>
#include <vector>

#include "config.hpp"
#include "utils.hpp"
#include "log.hpp"
#include "escape.hpp"

#include "graphics.hpp"
#include "backend.hpp"
#include "frontend.hpp"
#include "output.hpp"

// Args {{{
namespace Args {
  // {Anonymous} {{{
  namespace {
#define KEYS PROGRAM_FLAGS

    struct option longopts[] = {
#define X(A, F, G, B, C, E) {STR(B), C, nullptr, A},
        KEYS
#undef X
    };

    const char opts[] = {
#define X(A, F, G, B, C, E) A, F, G,
        KEYS
#undef X
    };

    void (*const help_funcs[])(std::ostream& s) = {
#define X(A, B, C, ...) C,
        FRONTEND_CONFIG
#undef X
    };

    void frontend_help(std::ostream& s) {
      for (int x = 0; x < FRONTEND_LENGTH; x++) {
        if (help_funcs[x] == nullptr)
          continue;
        help_funcs[x](s);
      }
    }

    void exec_help(std::ostream& s = std::cout) {
#define X(A, F, G, B, C, E) Utils::print_help(A, STR(B), E, true, s);
      KEYS
#undef X
    }

    namespace Keys {
      enum {
#define X(A, F, G, B, ...) B,
        KEYS
#undef X
      };
      int enumerate_key(int key) {

        switch (key) {
#define X(A, F, G, B, ...) \
  case A:                  \
    return Keys::B;
          KEYS
#undef X
              default : return -1;
        }
      }
    }

#undef KEYS
  }
  // }}}

  void print_version(std::ostream& s = std::cout) {
    s << "bfi++ " VERSION " (" __DATE__ ", " __TIME__ ") [" CXX " " __VERSION__
      << "]" << std::endl;
  }

  enum Get_Status {
    NONE,   ///< No input get status specified.
    FILE,   ///< Get input from file.
    TEXT,   ///< Get input from text.
    HELLO,  ///< Use default hello world text.
  };
  enum Graphics_Status {
    GS_NONE,
    GS_ENABLED,
    GS_DISABLED,
  };

  bool frontend_run = true;
  enum Graphics_Status graphical = GS_NONE;
  enum Frontend::Frontend_Index frontend = Frontend::SIMPLE_TEXT;
  enum Get_Status gstat = NONE;
  enum Output::Method output = Output::__NONE__;
  std::filesystem::path output_file = "";
  std::string text = "";

  Log::O verbose = {
      .v = 1,
      .lm = Log::SETTINGS,
  };
  Log::O error = {
      .e = true,
      .lm = Log::SETTINGS,
  };

  bool use_text() {
    if (text.empty() == true) {
      Log::print(error, "No code input specified.");
      return false;
    }
    return true;
  }

  const bool is_graphical[] = {
#define X(A, B, C, D, E, ...) D,
      FRONTEND_CONFIG
#undef X
  };
  const char* description[] = {
#define X(A, B, C, D, E, ...) E,
      FRONTEND_CONFIG
#undef X
  };

  void list_frontends(Log::O& v) {
    Log::print(v,
               "Listing available frontends.\n\tNOTE: case does not "
               "matter when choosing one.");

    v.fd << FRONTEND_DESCRIPTION "\n";
    for (int x = 0; x < FRONTEND_LENGTH; x++) {
      if (Frontend::functions[x].func == nullptr)
        continue;
      std::println(v.fd, "\t{:2}| {:16} - {}", x + 1,
                   Frontend::functions[x].name, description[x]);
    }
  }

  bool check_frontend(int num) {
    if (Frontend::functions[num].func == nullptr) {
      return false;
    }
    if (graphical == GS_DISABLED && is_graphical[num] == true) {
      Log::print(error, "Invalid frontend, graphics are disabled.");
      return false;
    }

    frontend = static_cast<Frontend::Frontend_Index>(num);
    return true;
  }

  bool choose_frontend(std::string method) {
    try {
      int num = std::stoi(method);
      if (num > 0 && num <= FRONTEND_LENGTH) {
        return check_frontend(num - 1);
      }
    } catch (std::invalid_argument& e) {}

    std::transform(method.begin(), method.end(), method.begin(), ::toupper);

    for (int x = 0; x < FRONTEND_LENGTH; x++) {
      if (Frontend::functions[x].name == method) {
        return check_frontend(x);
      }
    }

    return false;
  }

  enum Parse_Status {
    OK,      ///< Continue
    R_EXIT,  ///< Requested Exit
    ERROR,   ///< Error has occured in parsing
  };
  enum Parse_Status parse(int opt, char* arg, Log::O& e = error,
                          Log::O& v = verbose) {
    switch (opt) {
      // RUN AND KILL
      case Keys::longHelp:
        exec_help(v.fd);
        frontend_help(v.fd);
        return R_EXIT;
      case Keys::help:
        exec_help(v.fd);
        return R_EXIT;
      case Keys::version:
        print_version(v.fd);
        return R_EXIT;
        break;

        // INPUT
      case Keys::file:
        gstat = FILE;
        if (arg == nullptr) {
          Log::print(e, "Optarg was set to null. No file was provided.");
          return ERROR;
        }
        text = arg;
        Log::print(v, "Enabled input from file");
        break;
      case Keys::input:
        if (arg == nullptr) {
          gstat = HELLO;
          Log::print(v, "Set input to Hello World default");
          break;
        }
        gstat = TEXT;
        text = arg;
        if (text == "-" && graphical == false) {
          Log::print(v, "Reading from stdin for input");
          std::cin >> text;
          break;
        }
        Log::print(v, "Set argument as input");
        break;

        // FRONTEND
      case Keys::run:
        frontend = Frontend::NONE;
        Log::print(v, "Enabled run mode");
        break;
      case Keys::frontend:
        if (arg != nullptr) {
          if (choose_frontend(arg) == false) {
            Log::print(e, "Unable to find frontend method");
            return ERROR;
          }
          Log::print(v, "Changed frontend to ",
                     Frontend::functions[frontend].name);
          break;
        }
        [[fallthrough]];
      case Keys::listFronts:
        list_frontends(v);
        return R_EXIT;

        // OUTPUT
      case Keys::outputOnly:
        frontend_run = false;

        [[fallthrough]];
      case Keys::output:
        if (arg == nullptr) {
          Log::print(e, "Unable to find output file");
          return ERROR;
        }

        if (output == Output::__NONE__)
          output = Output::RAW;
        output_file = arg;
        break;

        // OTHER
      case Keys::verbose:
        Log::verbose_level = Log::verbose_max;
        Log::print(v, "Enabled Verbose");
        break;
      case Keys::gui:
        if (graphical == GS_DISABLED) {
          Log::print(error, "Graphics are disabled.");
          return ERROR;
        }
        graphical = GS_ENABLED;
        break;
      case Keys::noGui:
        graphical = GS_DISABLED;
        break;
      case -1:
        Log::print(e, "Unknown Option");
        return ERROR;
    }
    return OK;
  }

  void arguments(int argc, char* argv[]) {

    if (Utils::inTerminal() != true) {
      graphical = GS_ENABLED;
    }

    int opt = 0;
    while ((opt = getopt_long(argc, argv, opts, longopts, nullptr)) != -1) {
      if (opt == '?')
        exit(1);
      switch (parse(Keys::enumerate_key(opt), optarg)) {
        case OK:
          break;
        case R_EXIT:
          exit(0);
        case ERROR:
          exit(1);
      }
    }
  }

  void get_input(Backend& bf) {
    switch (gstat) {
      case NONE:
        Log::print(error, "No code input specified.");
        exec_help();
        exit(1);
      case FILE:
        if (use_text() == false)
          exit(1);
        if (!std::filesystem::exists(text)) {
          Log::print(Log::error(), "File does not exist.");
          exit(1);
        }
        Log::print(verbose, "Getting data from file");
        bf.get_file(text);
        break;
      case TEXT:
        if (use_text() == false)
          exit(1);
        Log::print(verbose, "Getting data from input");
        bf.get(text);
        break;
      case HELLO:
        bf.get();
        break;
    }
  }
}
// }}}
// Graphical {{{
namespace Graphical {
#ifndef DISABLE_GRAPHICS
#define MENU_POPUP_NAME "Menu"
  bool cleanup = true;
  bool help_window = false;
  static void menuHelpWindow() {
    if (help_window == false)
      return;
    ImGui::Begin("Help", &help_window, ImGuiWindowFlags_AlwaysAutoResize);

    static bool got_help_text = false;
    static std::string help_text = "";
    static std::vector<std::string> help_text_per;
    static std::vector<const char**> help_text_name;
    if (got_help_text == false) {
      std::stringstream ss;
      Args::exec_help(ss);
      help_text = ss.str();

      for (int x = 0; x < FRONTEND_LENGTH; x++) {
        if (Args::help_funcs[x] == nullptr)
          continue;
        ss.str(std::string());  // Clear String Stream
        Args::help_funcs[x](ss);
        help_text_per.push_back(ss.str());
        help_text_name.push_back(&Frontend::name[x]);
      }

      got_help_text = true;
    }

    // Help Terminal Flags
    if (ImGui::CollapsingHeader("Terminal Flags",
                                ImGuiTreeNodeFlags_DefaultOpen))
      ImGui::TextUnformatted(help_text.c_str());

    // Frontend Specific Help
    for (size_t x = 0; x < help_text_name.size(); x++) {
      if (ImGui::CollapsingHeader(*help_text_name[x])) {
        ImGui::TextUnformatted(help_text_per[x].c_str());
      }
    }

    ImGui::End();
  }

  static void menuMenu() {
    if (!ImGui::BeginMenuBar())
      return;

    if (ImGui::BeginMenu("Main")) {
      if (ImGui::MenuItem("Help")) {
        help_window = true;
      }
      if (ImGui::MenuItem("Quit"))
        exit(1);
      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }

  static bool appendFromDirectory(std::string file,
                                  std::vector<std::string>& file_names) {
    bool b = false;
    for (const auto& entry : std::filesystem::directory_iterator(file)) {
      const std::string e_str = entry.path().string();
      if (e_str.ends_with(".b")) {
        b = true;
        file_names.push_back(e_str);
      }
    }
    return b;
  }

  static void inputFileCombo() {
    static unsigned int files = 0;
    // 0) not initalized
    // 1) files found
    // 2) no files found
    if (files == 2)
      return;

    static std::vector<std::string> file_names{""};
    if (files == 0) {
      // Search for files
      bool t = false;
      if (appendFromDirectory("./", file_names) == true) {
        t = true;
      }

      if (std::filesystem::exists("./examples/") &&
          appendFromDirectory("./examples/", file_names) == true) {
        t = true;
      }

      if (t == true)
        files = 1;
    }

    static int select = 0;
    const char* preview = file_names[select].c_str();
    if (ImGui::BeginCombo("##input_file_combo", preview)) {
      for (int x = 1; x < (int)file_names.size(); x++) {
        const bool is_selected = (select == x);
        if (ImGui::Selectable(file_names[x].c_str(), is_selected)) {
          Args::text = file_names[x];
          select = x;
        }

        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
  }

  static void menuInputs(int& selected, int& input_select) {
    const char* input_status[3] = {"File", "Text", "Hello"};
    const char* input_status_description[3] = {
        "Get data from a file.",
        "Get data from the input text.",
        "Use the default Hello World Example.",
    };
    const char* preview = input_status[input_select];

    if (ImGui::BeginCombo("##input_combo", preview)) {
      for (int x = 0; x < 3; x++) {
        const bool is_selected = (input_select == x);
        if (ImGui::Selectable(input_status[x], is_selected)) {
          input_select = x;
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
          ImGui::SetTooltip(input_status_description[x]);
        }

        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }

    static std::string buf{"file.bf"};
    switch (input_select) {
      case 0:  // From File
        ImGui::InputText("Input File", &Args::text);
        Args::gstat = Args::FILE;
        inputFileCombo();
        break;

      case 1:  // From Input
        ImGui::InputTextMultiline(
            "Input Text", &Args::text,
            ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16));
        Args::gstat = Args::TEXT;
        break;

      case 2:  // Use Default Hello World
        Args::gstat = Args::HELLO;
        break;
    }

    if (ImGui::BeginListBox("##input_list")) {
      for (int x = 0; x < FRONTEND_LENGTH; x++) {
        const bool is_selected = (selected == x);
        if (ImGui::Selectable(Frontend::name[x], is_selected)) {
          selected = x;
        }

        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
          ImGui::SetTooltip(Args::description[x]);
        }

        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndListBox();
    }
  }

  static void menuOutputs(int& selected) {
    const char* preview = Output::name[selected];

    if (ImGui::BeginCombo("##output_combo", preview)) {
      for (int n = 0; n < OUTPUT_LENGTH; n++) {
        const bool is_selected = (selected == n);
        if (ImGui::Selectable(Output::name[n], is_selected))
          selected = n;

        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }

    static std::string buf{"file.bf"};
    ImGui::InputText("Output File", &buf);
    Args::output_file = buf;
  }

  void menuPopup(Graphics::Main_Function& mf) {
    static bool popup = true;
    static int input_select = 2;
    static int selected = 0;
    static std::string version = "";
    static unsigned int error = 0;

    if (popup == true) {
      ImGui::OpenPopup(MENU_POPUP_NAME);
      popup = false;

      std::stringstream ss;
      Args::print_version(ss);
      version = ss.str();
    }

    if (!ImGui::BeginPopupModal(MENU_POPUP_NAME, nullptr,
                                ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_MenuBar)) {
      return;
    }
    menuMenu();
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    menuHelpWindow();

    ImGui::Checkbox("Enable Frontend", &Args::frontend_run);
    if (Args::frontend_run == true) {
      menuInputs(selected, input_select);
    }

    static bool output = false;
    ImGui::Checkbox("Enable Output", &output);
    if (output == true) {
      menuOutputs(selected);
    }

    if (Graphics::centerButton("Run")) {
      if (input_select == 0 && !std::filesystem::exists(Args::text)) {
        error = 1;  // File not found
      } else {
        Args::check_frontend(selected);
        if (Args::gstat == Args::NONE)
          Args::parse(Args::Keys::input, nullptr);
        mf.e = false;  // Stop running this function
        if (Args::is_graphical[selected] == true)
          cleanup = false;
        Graphics::kill_me = true;
      }
    }

    static const char* errors[] = {"Error: File does not exist."};
    if (error != 0) {
      ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
      ImGui::TextWrapped(errors[error - 1]);
      ImGui::PopStyleColor();
    }

    /* Footer */

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() +
                         ImGui::GetContentRegionAvail().y - 175);
    ImGui::TextWrapped(
        "bfi++ is a brainf*** interpreter and virtual machine designed "
        "for "
        "adaptability and extensibility, offering multiple frontends to "
        "visualize memory and code execution. For program documentation "
        "please visit:");
    ImGui::TextLinkOpenURL("https://github.com/Oxygen-is-needed/bfipp",
                           nullptr);

    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    if (Graphics::centerButton("Exit")) {
      exit(1);
    }
    ImGui::Dummy(ImVec2(0.0f, 35.0f));
    ImGui::TextDisabled(version.c_str());

    ImGui::EndPopup();
  }

  void menu() {

    // Setup
    if (Graphics::setup() != true)
      return;

    // Main loop
    Graphics::mainFuncAdd(menuPopup);
    while (Graphics::main() == true) {}
    if (cleanup == false) {
      return;
    }

    // Cleanup
    Graphics::End::end();
  }
#else
  void menu() {
    Log::print(Args::error, "Graphical menu is Disabled");
  }
#endif
}
// }}}

int main(int argc, char* argv[]) {
  Unwind::initalize_unwind();
  Args::arguments(argc, argv);
  if (Args::graphical == Args::GS_ENABLED) {
    Graphical::menu();
  }

  // Set Settings
  Backend bf;
  if (Log::verbose_level == 2) {
    bf.print_rules();
  }

  // Get input
  Args::get_input(bf);

  // Convert
  Log::print({.v = 1, .lm = Log::BACKEND}, "Converting to IR representation");
  bf.convert();

  // Execute
  if (!Args::output_file.empty())
    Output::output(bf, Args::frontend, Args::output_file, Args::output);

  // Frontend
  if (Args::frontend_run == false)
    return 0;
  Log::print({.v = 1, .lm = Log::FRONTEND}, "Running frontend");
  Frontend::frontend(bf, Args::frontend);
}
// vim: tabstop=2 shiftwidth=2 softtabstop=2 expandtab
