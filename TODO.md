# v1.0.0 - Works with basic frontend

- [x] Backend
    - [x] VM
        - [x] Implement
        - [x] Use array for functions
        - [x] fix: `inspect_instructions()` crash; read outside backend.program string.
        - [x] fix: `get_file()`
        - [x] fix: interpreting bug
            - Using wrong variable to walk through instructions.
- [x] Fontend
    - [x] Basic Terminal
        - [x] Implement
        - [x] use: inspect Buffer
        - [x] add: inspect Insturctions
        - [x] Seperate frontend Functions
        - [x] Keybingings
            - [x] add: help text
            - [x] add: wait till instruction point
- [x] Utils
    - [x] Unwind

- [x] Main Program
    - [x] Add: options
        - [x] From file
        - [x] From cmd
        - [x] From stdin
        - [x] Run without a frontend
        - [x] Verbose: tell you what everything is before running it.
        - [x] version

- [x] Tests Pass
    - [x] Hello World
    - [x] [392quine.b](https://brainfuck.org/392quine.b)
    - [x] [bitwidth.b test](https://github.com/rdebath/Brainfuck/blob/master/bitwidth.b)
- [x] Package and Release
    - [x] add: makefile make releases
    - [x] add: logo
    - [x] add: github Releases
- [x] Wrap up
    - [x] Write README.md: Usage
    - [x] Detect Unused Code
    - [x] Look at TODO's
    - [x] Update Version Number

# v1.1.0 - Fixes and Improvements

- [x] Fix
    - [x] Fix critics
        - [x] Use `.hpp` instead of `.cpp`
        - [x] Remove implicit void arguments
        - [x] Use `static_cast` preferably instead of `c` type casting
        - [x] Prefer not to use char*
        - [x] Ues `cerr << '\n'` when single new line is needed.
            - Investigate better methods
        - [x] Use `nullptr` instead of `NULL`
        - [x] Prefer not to use anonymous namespaces
            - <https://stackoverflow.com/questions/4977252/why-an-unnamed-namespace-is-a-superior-alternative-to-static>
        - [x] Remove need of `using namespace std;`
    - [x] Remove debug macros
    - [x] Signal Catching

- [x] General
    - [x] Add/Fix Logging
        - [x] with verbose logging levels as well
        - [x] loging method prefix
        - [x] error appending after prefix
        - [x] replace all logs
        - [x] remove other two
        - [x] move to `log.hpp`
    - [x] add print if printable charactor when inspecting buffer.
    - [x] Update rules
        - [x] holding more rules
            - [x] length of tape
            - [x] what happens at end of tape
        - [x] Pass to VM as constructor
    - [x] Create VM destructor
    - [x] Easier way to add front
        - [x] add choose frontend
            - [x] number
            - [x] name
        - [x] list frontends
            - [x] add description of frontends
            - [x] list each
    - [x] Investigate mannor to load module for frontend/backend cross platform
        - will have to implement for each <https://blog.theopnv.com/posts/cpp-dynamic-loading/>
    - [x] Add Backend Output Methods
        - [x] Have frontend accept VM instead of backend
        - [x] Backend output whats neccessary for VM - vm.export()

- [x] Move configuration macros to `config.hpp`

- [x] Use Cmake instead of make

- [x] Documentation
    - [x] Improve README
        - [x] show default rules
        - [x] UPDATE: document how to add frontend
    - [x] `-H` | `--help-long` for longer help text.
        - [x] Normal Help
        - [x] Frontend Help
        - [x] Descriptions
    - [x] Examples
        - [x] How to use frontends


- [x] Wrap Up
    - [x] Clean up code
    - [x] Update README.md
    - [x] Update how to use program (eg `--help` output)
    - [x] Detect Unused Code
    - [x] Look at TODO's
    - [x] Update Version Number

- [x] Package and Release
    - [x] use cmake to create relaese in multiple formats
        - [x] Windows
        - [x] Linux
    - [x] add: github Releases

# v1.2.0 - Outputs and Graphics

- [x] Main
    - [x] Generalize `Args::arguments` to `Args::parse`
    - [x] Context Dependent `exit()` from `Args::parse`
    - [x] Can change stream to output
    - [x] `-G` | `--gui` argument to set graphical option
    - [x] Graphical Warning <https://github.com/raysan5/raygui>?
    - [x] Manuver to allow for wrapping or adding in frontends to graphics.

- [x] Output
    - [x] `-o` | `--output` argument specify output file.
    - [x] `-O` | `--only-output` argument specify output file, no run.
    - [x] Set frontend as NONE auto
    - [x] Output raw code
    - [x] Set file as executable
    - [x] Set frontend to be same as the one used at execution
    - [x] Open Graphical Menu if not running from terminal

- [x] Input
    - [x] See `#!` and ignore that line

- [x] Frontend
    - [x] Removable with macro
    - [x] Simple Text
        - [x] Wait till instruction type
        - [x] Clear output buffer
        - [x] Add base number to instruction index
    - [x] Simple Graphical Frontend
        - [x] See Buffer
        - [x] See instructions
        - [x] Next Instructions
        - [x] Step size
        - [x] Slowly Increment
        - [x] Default off

- [x] Document
    - [x] Add frontend function format (eg what input variable)


- [x] Wrap Up
    - [x] Support for Windows Compilation
    - [x] Add release compilation
    - [x] Update README.md
    - [x] Update Program Usage
    - [x] Detect Unused Code
    - [x] Look at TODO's
    - [x] use `clang-format`
    - [x] Update Version Number

# v1.3.0 - Graphical Update

- [x] Main
    - [x] `-N` | `--noGui` argument to hard set graphics disabled.
    - [x] Graphical menu
        - [x] help text
        - [x] show version
        - [x] select Text input
        - [x] select File input
        - [x] select output method
        - [x] output file name
        - [x] check if file exists
        - [x] list possible files ending with `.b` and/or in `examples/`
        - [x] output as file only

- [x] Frontend
    - [x] <new> Graphical frontend
        - [x] Buffer Grid
        - [x] Buffer Heat map
        - [x] Current Instructions (with middle enlarged)
        - [x] Color Border of middle instruction
        - [x] Center Text of instructions
        - [x] Center center instruction vertically
    - [x] Simple Text
        - [x] Run till spicific amount of steps.
    - [x] Simple Graphical
        - [x] Feature parity with simple text
           - [x] RUN: run entire program
           - [x] WAIT: wait till instruction index
           - [x] INSTRUCTION: wait till defined instruction
           - [x] ADVANCE: advance to step count
           - [x] Hover descriptions

- [x] Wrap Up
    - [x] Check for memory leaks
    - [x] Update release
    - [x] Update README.md
    - [x] Detect Unused Code
    - [x] Look at TODO's
    - [x] Syntax fix
    - [x] Update Version Number

# v1.4.0 - Improve Technicals

- [ ] Frontend
    - [ ] Have multiple graphics windows at once
    - [ ] Get input for program
    - [ ] Graphical frontend
        - [ ] Have grid expand if needed.
        - [ ] click open menu on what to do about instruction
    - [ ] Simple Graphical frontend
        - [ ] Clean up code
    - [ ] Simple Text
        - [ ] Modify Advance (+num) to add to pc
    - [ ] Ability to Disable a backends
    - [ ] Ability to Disable Graphics

- [ ] Main
    - [ ] Allow multiple VM's running.
    - [ ] Open menu after running.
    - [ ] Have multiple methods running at once.
    - [ ] Have backbone to support multiple graphical windows.

# Ideas

- ALL
    - Move Define Configuration options macros to "config.hpp"
    - Write docs on creating frontends
    - Write docs on creating backend
- Main
    - Option to output parsed file
    - REPL for BF
    - Change bf rules
    - output/input bf rules from file
- Backend
    - Remove need for seek and seekb operations.
        - Have rule for the charactors to seek in those functions
    - Step Back one step
    - Optimize
    - Convert necessary variables to unsigned long
    - Add input buffer
    - Support for multicharactor tokens
    - Support for Huffman Coding binary files.
    - Other Langs
        - Support for bf++?: https://esolangs.org/wiki/BF%2B%2B
        - Support for boolf\*\*k? https://esolangs.org/wiki/Boolfuck
        - Support for sembly? https://esolangs.org/wiki/Sembly
- Frontend
    - Simple Text
        - Input buffer?
        - Wait till break out of loop.
        - add keybinds to extend inspect
