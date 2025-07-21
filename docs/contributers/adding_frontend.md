# Creating a new frontend

## Steps

1. Develop a new function that takes the Backend class as an argument.
2. Include the function and the macro named `FRONTEND_CONFIG` in `config.hpp`.
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


## Adding to `FRONTEND_CONFIG`

Next, you need to incorporate your new function into the `FRONTEND_CONFIG` macro
located in `config.hpp`. This task is quite straightforward. Simply create a
brief name for your frontend (preferably 8 to 12 characters long) and add it to
the macro list in all uppercase letters. The case of the name does not matter
when choosing it within the program. Next to it, include the function name. It
should look like this:

```cpp
#define FRONTEND_CONFIG \
  X(YOUR_FRONT, YOUR_FRONT::frontend)
```
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


