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



## BF Default Rules


Execution Rules | Charactor | Name | Description
:-:|:-:|:-:|---
 1 | `+` | Increment | Increment current cell
 2 | `-` | Decremet | Decrement current cell
 3 | `>` | Move Right | Move to next cell
 4 | `<` | Move Left | Move to previous cell
 5 | `[` | Start Loop | Checks if current cell is zero, then moves to matching bracket `]`
 6 | `]` | End Loop | Checks if current cell is non-zero, then moves back to maching bracket `[`
 7 | `.` | Print | Outputs the value of the current cell
 8 | `,` | Input | Takes a single char and puts its value in the current cell

VM Rules | Values | Description
:-:|:-:|---
 1 | `tape_length` [uint] | The length of the buffer, used in execution
 2 | `eot_wrap` [true/false] | If at either end buffer to wrap to the other end


## Documentation

- [contributers](./docs/contributers/)
    - [adding frontends](./docs/contributers/adding_frontend.md)


## Contribution

Suggestions and General Contribution are Welcome. This is a learning project so
be prepared to be bombarded with questions, and potentially for it to be
re-written.

## References

- [Wikipedia](https://en.wikipedia.org/wiki/Brainfuck)
- [BF Website](https://brainfuck.org/)
- [BF Torture Test + More](https://github.com/rdebath/Brainfuck)
