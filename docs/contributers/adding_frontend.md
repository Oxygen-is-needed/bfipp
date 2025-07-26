# Creating a new frontend

## Steps

1. Develop a new function(s) that takes the VM class reference as an argument.
2. Include the functions in the macro named `FRONTEND_CONFIG` in `config.hpp`.
3. Include disable option in `CMakeLists.txt`.

The most challenging aspect will be creating a new function that accepts the
VM class reference. This is primarily because you'll need to conduct extensive
research to understand how to interact with this class, along with the overall
effort involved. However, this guide aims to provide a straightforward
introduction to the process.

## Implementing the VM Interface Function

### Location/Naming

The code for this function should reside within a namespace. You can place the
namespace anywhere before the inclusion of `frontend.hpp` or even inside it. If
you wish to utilize the utilities available for the frontends, ensure you place
it before the `Frontend_Utils` namespace in `frontend.hpp`.

The function should be named "frontend" and placed within its corresponding
namespace. The namespace must follow Pascal case conventions and should be the
extended version of the name that will be used when adding it to the
`FRONTEND_CONFIG`.

### Implementing Frontend Disabling

To remove a frontend during compilation, simple macros are utilized. The naming
convention for these macros is `DISABLE_FRONTEND__` followed by the name you
assigned to your frontend in `config.hpp`.

Once you've chosen a name, you can use the following macros within the namespace
you created for your frontend:

```cpp
namespace YourFrontend {
#ifndef DISABLE_FRONTEND__YOUR_FRONTEND

// ... your code here

#else
    void (*const frontend)(VM& vm) = nullptr;
    void (*const help)() = nullptr;
#endif
}
```


This code snippet sets the function pointers to `nullptr`, allowing the program
to ignore them when checking for their existence.

### Interfacing with the VM Class

To effectively interface with the `VM` class, it can be helpful to review some
examples, as the process mainly involves a straightforward loop, akin to a REPL
loop:

1. Read the user input
2. Execute the action
3. Advance the VM
4. Repeat

When connecting with the `VM` class, there are several key variables to keep in
mind:

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

Additionally, here are the `VM` rules to consider:

| VM Rules| Location| Description|
|:-:|:-:|---|
| Tape length| `.rules.tape_length` (unsigned int) | Length of the VM buffer|
| Wrap| `.rules.eot_wrap` (bool)| Indicates if the tape should wrap at either end|
| Execution rules| `.rules.r[]` (std::vector<Rule>)| Defines actions for each command character|
| Rule commands| `.rules.r[].cmd` (char)| Character interpreted as an action|
| Rule binary| `.rules.r[].findex` (std::vector<unsigned char>) | Function array index list for command actions |

You can leverage these values from the `VM` class to obtain nearly all the
necessary information for creating a frontend.

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

## Updating `CMakeLists.txt`

Once everything is set up, you'll need to modify the `CMakeLists.txt` file to
enable CMake to disable your frontend. There are two specific sections in this
file where you'll need to make additions:

1. Under the `Frontends Disable Options` comment
2. Under the `Disable Frontends Macros` comment

### Step One

This part is straightforward. You'll create an option name by prefixing
`DISABLE_` with the name of your function from `config.hpp`, converting it to
uppercase and separating words with underscores. It should look like this:

```cmake
option(DISABLE_YOUR_FRONTEND "Disable Your Frontend" OFF)
```

Place this option in the same location as it appears in `config.hpp`. By
default, you want your frontend to be included, so **OFF** is the default
selection. If you prefer it to be disabled by default, change **OFF** to **ON**.

### Step Two

In this section, you'll create an *if* statement using the name you defined in
the previous step. Then, add the necessary macro to disable your frontend, which
was created during the frontend setup. It should resemble the following:

```cmake
if (DISABLE_YOUR_FRONTEND)
    target_compile_definitions(bfi++ PRIVATE
        "-DDISABLE_FRONTEND__YOUR_FRONTEND")
endif()
```

If you have headers that are not used elsewhere, you can ensure they are
included when your option is not declared, or include any other options your
frontend requires. It should look something like this:

```cmake
if (DISABLE_YOUR_FRONTEND)
    target_compile_definitions(bfi++ PRIVATE
        "-DDISABLE_FRONTEND__YOUR_FRONTEND")
else()
    # ... your options
endif()
```
