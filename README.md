<div align="center">

# bfi++
##### A adaptable and extendable BF interpreter.


<img alt="bfi++" src="img/logo.svg" width=50%></img>
</div>


## Program Usage

```
    -f | --file      - file: Input file.
    -h | --help      - Prints helps text.
    -i | --input     - [txt]: Input text. Default with no arguments.
                        '-' for reading from stdin.
    -r | --run       - Run input imediatly.
    -V | --verbose   - Enable verbose output.
    -v | --version   - Print version
```



## BF Default Syntax

- File should end in ".b".
- Each charactor in file is either a command or ignored.
    <!-- NOTE: Check for Extended Ascii Charactors? -->
- Array of 30,000 of intager values (minimally one byte).
    - Inital value of pointer of zero.
- Control flow starts at first.
    - When no more or EOF program terminates.
- Instructions:
    - "+" Increment
    - "-" Decrement
    - ">" Move Right
        - At Right Most Cell "Unpredictable"
    - "<" Move Left
        - At Right Most Cell "Unpredictable"
    - Loops
        - "[" Start Loop
            - If value at pointer is Zero, goto End Loop.
        - "]" End Loop
            - If value at pointer is Non-Zero, goto Start Loop.
        - Using a Stack would work well.
        - These Jumps corrispond to a match bracket.
        - Does not need to add +1 to the PC, since the conditions should always
          be the same when jumping between the two instructions.
    - I/O
        - "." Print
            - May be Modulo 256.
        - "," Input
        - Many different rules here for input from files and output to files.


## Contribution

Suggestions and General Contribution are Welcome. This is a learning project so
be prepared to be bombarded with questions, and potentially for it to be
re-written.

## References

- [Wikipedia](https://en.wikipedia.org/wiki/Brainfuck)
- [BF Website](https://brainfuck.org/)
- [BF Torture Test + More](https://github.com/rdebath/Brainfuck)
