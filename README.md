# Mega Shell — System Design

## Overview

Mega Shell is a custom Unix-like shell implemented in C for educational purposes.

The project is designed to help understand:

* C programming fundamentals
* Linux/macOS system calls
* Terminal behavior
* Process management
* Shell internals

The shell currently supports:

* Interactive command loop
* Built-in commands (`cd`, `pwd`, `echo`, `exit`)
* Raw terminal mode
* Command history
* Arrow key navigation
* Input parsing

---

# High-Level Architecture

The shell follows a layered pipeline:

```text
Keyboard Input
    ↓
Terminal Layer
    ↓
Parser Layer
    ↓
Command Dispatcher
    ↓
Built-in / External Command Execution
```

---

# System Components

## 1. Terminal Layer (`terminal.c`)

Responsible for all terminal interaction and user input behavior.

### Responsibilities

* Enable raw terminal mode
* Disable canonical input mode
* Disable terminal echo
* Read user input character-by-character
* Handle backspace
* Handle arrow keys
* Maintain command history
* Render command line manually

### Core Functions

### `enable_raw_mode()`

Configures terminal settings:

* disables `ICANON`
* disables `ECHO`

This allows immediate key processing.

---

### `disable_raw_mode()`

Restores original terminal state before shell exits.

Important for preventing terminal corruption after program termination.

---

### `read_input()`

Reads input one byte at a time.

Handles:

* Enter
* Backspace
* Up arrow history
* Down arrow history
* Left/right arrow ignoring

Returns:

```c
char* input
```

representing the full command line.

---

### `add_to_history()`

Stores previously executed commands.

History is stored globally:

```c
history[HISTORY_SIZE]
history_count
```

---

### `clear_line()`

Erases the currently displayed command line.

Used during history navigation.

---

---

## 2. Parsing Layer (`input_parser.c`)

Responsible for converting raw command strings into structured tokens.

### Input

```text
echo hello world
```

### Output

```c
["echo", "hello", "world", NULL]
```

---

### Core Function

### `parse_input()`

Splits input by:

* spaces
* tabs
* newlines
* carriage returns

Allocates memory dynamically for tokens.

---

### `free_tokens()`

Frees token arrays after command execution.

Prevents memory leaks.

---

---

## 3. Command Dispatch Layer (`main.c`)

Responsible for routing commands to the appropriate implementation.

### Workflow

Main shell loop:

1. Display prompt
2. Read input
3. Save command to history
4. Parse input
5. Dispatch command
6. Free memory

---

### `shell_loop()`

Main infinite loop:

```c
while (1)
```

This is the event loop of the shell.

---

### `shell_build()`

Dispatches commands based on first token:

```c
args[0]
```

Examples:

* `cd`
* `pwd`
* `echo`
* `exit`

---

Example dispatch:

```c
if (my_strcmp(args[0], "pwd") == 0)
```

Routes to:

```c
command_pwd()
```

---

---

## 4. Built-in Command Layer (`builtins.c`)

Implements shell-native commands.

These commands execute inside the shell process itself.

Why?

Because commands like `cd` must modify shell state.

Example:

A child process changing directory would not affect parent shell.

---

### Implemented Built-ins

---

### `command_cd()`

Changes current working directory using:

```c
chdir()
```

Supports:

* `cd`
* `cd <path>`

Uses:

```c
getenv("HOME")
```

for default home directory.

---

### `command_pwd()`

Prints current directory using:

```c
getcwd()
```

---

### `command_echo()`

Prints user arguments.

Supports:

* `echo hello`
* `echo -n hello`

Planned:

* environment variable expansion

---

### `exit`

Restores terminal state and terminates shell.

---

---

## 5. Utility Layer (`utils.c`)

Contains helper functions.

---

### `my_strcmp()`

Custom implementation of:

```c
strcmp()
```

Used for command comparisons.

---

# Data Flow

## Command Execution Lifecycle

Example:

```bash
echo hello
```

Execution path:

```text
User types command
    ↓
read_input()
    ↓
add_to_history()
    ↓
parse_input()
    ↓
shell_build()
    ↓
command_echo()
```

---

# Memory Management Strategy

The shell dynamically allocates memory for:

* input buffers
* parsed tokens
* history entries

Freed resources:

* parsed tokens after execution
* input buffers after parsing

Persistent allocations:

* command history

---

# Terminal Behavior

The shell uses raw mode:

```c
ICANON disabled
ECHO disabled
```

This allows:

* immediate key handling
* manual echoing
* arrow key detection

Arrow sequences handled:

| Key   | Sequence |
| ----- | -------- |
| Up    | ESC [ A  |
| Down  | ESC [ B  |
| Right | ESC [ C  |
| Left  | ESC [ D  |

---

# Current Limitations

Not yet implemented:

* External commands (`ls`, `cat`, etc.)
* PATH resolution
* Pipes (`|`)
* Redirection (`>`, `<`)
* Background jobs (`&`)
* Signal handling
* Tab completion

---

# Planned Features

## External Commands

Will use:

* `fork()`
* `execvp()`
* `waitpid()`

---

## Pipes

Will use:

* `pipe()`
* `dup2()`

---

## Redirection

Will use:

* `open()`
* `dup2()`

---

## Signals

Will handle:

* `SIGINT`
* `Ctrl+C`

---

# Design Principles

The project follows these system design principles:

---

## Separation of Concerns

Modules are separated by responsibility:

* terminal handling
* parsing
* dispatching
* command execution

---

## Modularity

Files are organized as:

* `main.c`
* `terminal.c`
* `input_parser.c`
* `builtins.c`
* `utils.c`

---

## Explicit Resource Management

Manual management of:

* memory
* terminal state
* buffers

---

## Layered Architecture

Pipeline:

```text
Input → Parse → Dispatch → Execute
```

This design is intentionally similar to real shell systems.

---

# Future Refactoring

Planned improvements:

* command table instead of long if/else chain
* shell state struct instead of global variables
* richer command structures for pipes/redirection

Example future command representation:

```c
struct command {
    char** args;
    char* input_file;
    char* output_file;
    int background;
};
```

---

# Learning Outcomes

This project teaches:

* pointers
* dynamic memory
* terminal APIs
* system calls
* process creation
* file descriptors
* shell architecture
* Unix programming concepts

# Notes for me:
### System Design Prenciples:
- Single Responsibility Principle (SRP): For example, a function should do one job.
- Open/Closed Principle (OCP): You can add new functionality without changing the existing code. You can achieve this by creating a common interface.
- Dependency Inversion Principle (DIP): 
- Law of Demeter (LoD): A module should not know the inner details of the objects it manipulates.