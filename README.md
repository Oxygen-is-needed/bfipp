<div align="center">

# bfi++
##### A adaptable and extendable BF interpreter.


<img alt="bfi++" src="img/logo.svg" width=50%></img>
</div>

`bfi++` is a Brainf\*\*\* interpreter and virtual machine designed for
adaptability and extensibility, offering multiple frontends to visualize memory
and code execution.

> [!WARNING]
>
> Project is not fully completed, nor documented, and is prone to frequent
> changes. Proceed at your own risk.

## Program Usage

```
        -h | --help          - Prints helps text.
        -H | --longHelp      - Prints extra helps text.
        -V | --verbose       - Enable verbose output.
        -v | --version       - Print version

        -f | --file          - file: Input file.
        -i | --input         - [txt]: Input text. Default with no arguments.
                                Use '-' for reading from stdin.
        -r | --run           - Run input imediatly, using no frontend.

        -l | --listFronts    - List all available frontends.
        -F | --frontend      - Change the frontend used to render.
                                Use no arguments to list frontends.
        -o | --output        - Output code as file. Will run frontend.
        -O | --outputOnly    - Only output code as file. Will not run frontend.
        -G | --gui           - [Not Working] Run with GUI menu.
```


## Quickstart

To get started quickly using the program use the following arguments:

```bash
./bfipp -i
```

This will use the default frontend (SimpleText) and the hello world program.
This will get you the bare minimum to get it running. To get more information
use the following commands.

```bash
./bfipp -h  # Prints basic help arguments
./bfipp -H  # Prints extended help, including help for the frontends
```

For more program to run here are some locations to look:

- <http://www.brainfuck.org/>
- <https://github.com/rdebath/Brainfuck/>


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
- [Docs for ImGui](https://pixtur.github.io/mkdocs-for-imgui/site/)
