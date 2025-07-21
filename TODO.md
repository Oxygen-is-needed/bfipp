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

# v1.1.0

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
            - https://stackoverflow.com/questions/4977252/why-an-unnamed-namespace-is-a-superior-alternative-to-static
        - [x] Remove need of `using namespace std;`
    - [x] Remove debug macros
    - [x] Signal Catching

- [ ] General
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
    - [ ] Easier way to add front/backend
    - [ ] Investigate mannor to load module for frontend/backend

- [x] Move configuration macros to `config.hpp`

- [ ] Use Cmake instead of make

- [ ] Documentation
    - [ ] Improve README
        - [x] show default rules
        - [ ] document how to add frontend
        - [ ] document how to add backend
    - [ ] `-H` | `--help-long` for longer help text.

- [ ] Package and Release
    - [ ] use cmake to create relaese in multiple formats
        - [ ] Windows
        - [ ] Linux
    - [ ] add: github Releases

- [ ] Wrap Up
    - [ ] Clean up code
    - [ ] Update README.md
    - [ ] Detect Unused Code
    - [ ] Look at TODO's
    - [ ] Update Version Number




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
        - Add base number at end of numbers, to know index of each. (e.g. '1''2''3''4' 10 = 11, 12, 13, 14)
        - Run till spicific amount of steps.
        - Input buffer?
        - Wait till break out of loop.
        - add keybinds to extend inspect
