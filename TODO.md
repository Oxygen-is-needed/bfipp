# v1.0.0 - Works with basic frontend

- [x] Backend
    - [x] VM
        - [x] Implement
        - [x] Use array for functions
        - [x] fix: inspect_instructions() crash; read outside backend.program string.
        - [x] fix: get_file()
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

- [ ] Main
    - [ ] If run in graphical mode, open terminal.

- [x] Output
    - [x] `-o` | `--output` argument specify output file.
    - [x] `-O` | `--only-output` argument specify output file, no run.
    - [x] Set frontend as NONE auto
    - [x] Output raw code
    - [x] Set file as executable
    - [x] Set frontend to be same as the one used at execution

- [x] Input
    - [x] See `#!` and ignore that line

- [ ] Frontend
    - [ ] Removable with macro
    - [ ] Simple Text
        - [ ] Wait till instruction type
        - [ ] Clear output buffer
        - [ ] Add base number to instruction index
    - [ ] Simple Graphical Frontend (raylib)

- [ ] Document
    - [ ] Add frontend function format (eg what input variable)


- [ ] Wrap Up
    - [ ] Update README.md
    - [ ] Update Program Usage
    - [ ] Detect Unused Code
    - [ ] Look at TODO's
    - [ ] Update Version Number
    - [ ] Package and Release


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
    - Heat map for which tokens are access the most.
    - Simple Text
        - Run till spicific amount of steps.
        - Input buffer?
        - Wait till break out of loop.
        - add keybinds to extend inspect
