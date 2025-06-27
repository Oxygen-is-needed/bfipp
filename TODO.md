# v1.0.0 - Works with basic frontend

- [ ] Backend
    - [ ] VM
        - [x] Implement
        - [x] Use array for functions
        - [x] fix: inspect_instructions() crash; read outside backend.program string.
        - [x] fix: get_file()
        - [ ] fix: interpreting bug
        - [ ] fix: receiving input
- [x] Fontend
    - [x] Basic Terminal
        - [x] Implement
        - [x] use: Inspect Buffer
        - [x] add:use: Inspect Insturctions
        - [x] Seperate frontend Functions
        - [x] Keybingings
            - [x] add: help text
            - [x] add: wait till instruction point
- [x] Utils
    - [x] Unwind

- [ ] Main Program
    - [ ] Add options
        - [x] From file
        - [x] From cmd
        - [x] From stdin
        - [x] Run without a frontend
        - [ ] Verbose: tell you what everything is before running it.

- [ ] Tests Pass
    - [x] Hello World
    - [x] [392quine.b](https://brainfuck.org/392quine.b)
    - [ ] [bitwidth.b test](https://github.com/rdebath/Brainfuck/blob/master/bitwidth.b)
- [ ] Package and Release
    - [ ] add: makefile make releases
    - [ ] Create Appimage
    - [x] Logo
    - [ ] add: github Releases
- [ ] Wrap up
    - [ ] Look at TODO's

# Ideas

- ALL
    - Move Define Configuration options macros to "config.hpp"
- Main
    - Option to output parsed file
    - REPL for BF
- Backend
    - Step Back one step
    - Optimize
    - Convert necessary variables to unsigned long
    - Add output buffer
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
