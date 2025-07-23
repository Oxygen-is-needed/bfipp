# Creating a new frontend

## Steps

1. Develop a new function(s) that takes the Backend class as an argument.
2. Include the functions in the macro named `FRONTEND_CONFIG` in `config.hpp`.
3. That's it!

The most challenging aspect will be creating a new function that accepts the
Backend class. This is primarily because you'll need to conduct extensive
research to understand how to interact with this class, along with the overall
effort involved. However, this guide aims to provide a straightforward
introduction to the process.


## Implementing the Backend Interface Function

### Location/Naming

The code for this function should reside within a namespace. You can place the
namespace anywhere before the inclusion of `frontend.hpp` or even inside it. If
you wish to utilize the utilities available for the frontends, ensure you place
it before the `Frontend_Utils` namespace in `frontend.hpp`.

The function should be named "frontend" and placed within its corresponding
namespace. The namespace must follow Pascal case conventions and should be the
extended version of the name that will be used when adding it to the
`FRONTEND_CONFIG`.

### Interfacing with the Backend Class

To interface with the `Backend` class, reviewing some examples may be
beneficial, as the process primarily involves a simple loop, similar to a REPL
loop.

1. Read the user input
2. Execute the action
3. Advance the VM
4. Repeat

The key components necessary for connecting to the backend are found within the
`VM` that the backend contains. To gain direct access to the `VM`, create a
variable reference to it and call the `.get_vm()` function on the backend. This
can be done as follows:

```cpp
VM& vm = backend.get_vm();
```

Once you have this reference, you can interact with the VM directly, saving time
compared to navigating through the Backend. To connect with the `VM` class,
there are several important variables to consider:

| Instruction| Location|
|:-:|---|
| Instruction list| `.instructions` (std::vector<unsigned char>) |
| VM buffer| `.buffer` (unsigned char[?])|
| Output buffer| `.output` (std::string)|
| VM rules| `.rules` (Rules)|
| Instruction index| `.ins_i`|
| Total instructions| `.ins_max`|
| Total steps| `.total_steps`|
| Buffer index| `.pc`|

| VM Rules| Location| Description|
|:-:|:-:|---|
| Tape length| `.rules.tape_length` (unsigned int) | Length of the VM buffer|
| Wrap| `.rules.eot_wrap` (bool)| Indicates if the tape should wrap at either end|
| Execution rules| `.rules.r[]` (std::vector<Rule>)| Defines actions for each command character|
| Rule commands| `.rules.r[].cmd` (char)| Character interpreted as an action|
| Rule binary| `.rules.r[].findex` (std::vector<unsigned char>) | Function array index list for command actions |

You can utilize these values, derived from the `VM` class to get pretty much all
neccessary information for creating a frontend.


### Creating a help function (Optional)

Since this is optional, the implementation will remain more general.

To create a help function, you can utilize the keys macro along with the
corresponding descriptions for each key, and employ the `Utils::print_help()`
function. This can be implemented as follows:

```cpp
#define FRONTEND_KEYS__YOUR_FRONT \
    X(QUIT, 'q', "Quit")

#define X(A,B,C)    Utils::print_help, B, "", C, false);
    FRONTEND_KEYS_YOUR_FRONT
#undef  X
```


## Adding to `FRONTEND_CONFIG`

Next, you need to integrate your new function into the `FRONTEND_CONFIG` macro
found in `config.hpp`. This process is quite simple. Just create a short name
for your frontend (ideally between 8 to 12 characters) and add it to the macro
list in uppercase letters. The case of the name is not significant when
referenced in the program.

Alongside it, include the function name, which serves as the entry point for the
frontend. You can also add either your help function or a `nullptr` if you
prefer to omit it.

The final argument is the description of your frontend, which will be used when
listing the available frontends. If you wish to skip this, you can simply use
`""` instead of providing an actual description.

This could resemble the following example, with the first being the bare
minimum:

```cpp
#define FRONTEND_CONFIG \
  X(NONE, None::frontend, nullptr, "") \
  X(YOUR_FRONT, YourFront::frontend, YourFront::help, "Your frontend description")
```
