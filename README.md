# Mega Shell — Detailed Architecture Specification

---

# Project Goal

Mega Shell is a Unix-like shell written in C for learning:

* C programming
* Operating systems
* terminal internals
* process management
* system calls
* provenance tracking
* command history persistence

---

# Directory Structure

```text id="glb1rz"
mega-shell/
│
├── src/
│   ├── controllers/
│   ├── models/
│   ├── views/
│   ├── services/
│   ├── builtins/
│   ├── utils/
│   └── main.c
│
├── include/
│   └── my_shell.h
│
├── data/
│   ├── history.db
│   └── provenance.db
│
├── Makefile
└── README.md
```

---

# 1. MAIN ENTRYPOINT

---

## `src/main.c`

Application startup.

Responsible only for bootstrapping the shell.

---

## Functions

---

### `int main(int argc, char** argv, char** env)`

### Responsibilities

* initialize shell state
* initialize databases
* enable raw mode
* start shell loop
* cleanup on exit

### Flow

```text id="6z0w3u"
initialize_shell()
start_shell_loop()
cleanup_shell()
```

---

---

# 2. CONTROLLERS

Controllers orchestrate program flow.

---

## `src/controllers/shell_controller.c`

Main shell controller.

Owns interactive loop.

---

## Functions

---

### `void shell_loop(char** env, ShellState* shell_state)`

### Responsibilities

* render prompt
* read input
* save history
* parse input
* dispatch command
* log provenance
* free memory

---

Flow:

```text id="2r8j5p"
render prompt
read input
parse
execute
log
repeat
```

---

---

### `void initialize_shell(ShellState* shell_state)`

### Responsibilities

* initialize shell state values
* load history
* initialize databases

---

---

### `void cleanup_shell(ShellState* shell_state)`

### Responsibilities

* disable raw mode
* free history memory
* close databases

---

---

## `src/controllers/command_controller.c`

Dispatches commands.

---

## Functions

---

### `int dispatch_command(char** args, char** env, ShellState* shell_state)`

### Responsibilities

* identify command type
* call builtin handler
* call external executor

---

Flow:

```text id="r3g7yf"
if builtin:
   run builtin
else:
   run external command
```

---

---

### `int is_builtin(char* command_name)`

### Responsibilities

Determine whether command is shell-native.

Returns:

```c id="0l5vh0"
1 = builtin
0 = external
```

---

---

### `int execute_builtin(char** args, char** env, ShellState* shell_state)`

Routes built-in commands.

---

---

# 3. MODELS

Models manage persistent and runtime data.

---

## `src/models/shell_state.c`

Stores runtime shell state.

---

## Data Structure

---

### `typedef struct ShellState`

Fields:

```c id="sw5k7n"
typedef struct {
    char* current_directory;
    char* previous_directory;
    int last_exit_status;
    int history_count;
} ShellState;
```

---

## Functions

---

### `ShellState* create_shell_state()`

Allocates and initializes shell state.

---

---

### `void destroy_shell_state(ShellState* shell_state)`

Frees shell state memory.

---

---

## `src/models/history_model.c`

Handles command history.

Persistent storage layer.

Uses SQLite.

---

## Functions

---

### `void initialize_history_db()`

Creates history database if absent.

Creates table:

```sql id="p9l53e"
history(
    id INTEGER PRIMARY KEY,
    command TEXT,
    timestamp DATETIME
)
```

---

---

### `void save_command_to_history(char* command)`

Insert executed command into DB.

---

---

### `char* load_history_command(int index)`

Retrieve command by history index.

Used by arrow navigation.

---

---

### `void free_history()`

Free in-memory history cache.

---

---

## `src/models/provenance_model.c`

Tracks system provenance.

Stores process relationships and file interactions.

Uses SQLite.

---

## Database Tables

---

### `commands`

```sql id="yo1j6w"
commands(
    id INTEGER PRIMARY KEY,
    command TEXT,
    timestamp DATETIME
)
```

---

### `processes`

```sql id="2b7db6"
processes(
    pid INTEGER,
    parent_pid INTEGER,
    command_id INTEGER
)
```

---

### `files`

```sql id="mjlwm2"
files(
    id INTEGER PRIMARY KEY,
    path TEXT
)
```

---

### `interactions`

```sql id="jvvh6s"
interactions(
    process_id INTEGER,
    file_id INTEGER,
    interaction_type TEXT
)
```

---

## Functions

---

### `void initialize_provenance_db()`

Creates provenance database.

---

---

### `void log_command_execution(char* command)`

Logs user command.

---

---

### `void log_process_spawn(pid_t pid, pid_t parent_pid, char* command)`

Logs spawned process.

---

---

### `void log_file_interaction(pid_t pid, char* filepath, char* interaction_type)`

Logs file read/write interaction.

---

---

# 4. VIEWS

View layer handles terminal interface.

---

## `src/views/terminal_view.c`

Terminal behavior.

Current raw mode implementation belongs here.

---

## Functions

---

### `void enable_raw_mode()`

Disables:

* canonical mode
* echo

---

---

### `void disable_raw_mode()`

Restores terminal settings.

---

---

### `char* read_input()`

Reads input character-by-character.

Supports:

* Enter
* Backspace
* Up arrow
* Down arrow
* Left arrow
* Right arrow

Returns input string.

---

---

### `void clear_line(int* position)`

Erase current input visually.

---

---

## `src/views/prompt_view.c`

Prompt rendering.

---

## Functions

---

### `void render_prompt()`

Display:

```text id="gj1hdo"
[current_directory]>
```

---

Future:

* colors
* git branch
* status code

---

---

# 5. SERVICES

Business logic layer.

---

## `src/services/parser_service.c`

Parser logic.

---

## Functions

---

### `char** parse_input(char* input)`

Tokenize command.

Input:

```text id="37q3gd"
echo hello world
```

Output:

```c id="e7fmsf"
["echo", "hello", "world", NULL]
```

---

---

### `void free_tokens(char** tokens)`

Free parser output.

---

---

Future:

---

### `Command* parse_command_structure(char* input)`

Rich parser for:

* pipes
* redirection
* background jobs

---

---

## `src/services/executor_service.c`

External command execution.

---

## Functions

---

### `int execute_external_command(char** args, char** env)`

Uses:

* fork
* execvp
* waitpid

---

Flow:

```text id="g35w78"
fork
child executes
parent waits
```

---

---

### `char* resolve_command_path(char* command, char** env)`

Search executable inside PATH.

---

Used for:

```bash id="m0jx13"
which ls
```

---

---

## `src/services/terminal_service.c`

Terminal helper functions.

---

## Functions

---

### `int is_arrow_key(char c)`

Detect escape sequences.

---

---

### `void move_cursor_left()`

Future cursor editing.

---

---

### `void move_cursor_right()`

Future cursor editing.

---

---

# 6. BUILTINS

Each builtin isolated in separate file.

---

## `src/builtins/cd.c`

---

### `int command_cd(char** args, ShellState* shell_state)`

Supports:

* cd
* cd path
* cd -
* cd ~

Updates shell state.

---

---

## `src/builtins/pwd.c`

---

### `int command_pwd()`

Print current directory.

---

---

## `src/builtins/echo.c`

---

### `int command_echo(char** args, char** env)`

Supports:

* echo
* echo -n
* environment variables

---

---

## `src/builtins/env.c`

---

### `int command_env(char** env)`

Print environment variables.

---

---

## `src/builtins/which.c`

---

### `int command_which(char** args, char** env)`

Locate executable.

---

---

## `src/builtins/exit.c`

---

### `int command_exit()`

Cleanup and terminate shell.

---

---

# 7. UTILS

Helper utilities.

---

## `src/utils/string_utils.c`

---

### `int my_strcmp(const char* str1, const char* str2)`

String comparison.

---

---

### `char* my_strdup(const char* str)`

Duplicate string.

---

---

### `int my_strlen(const char* str)`

String length.

---

---

### `char* my_strcpy(char* dest, const char* src)`

Copy string.

---

---

# Future Data Structures

---

## `Command`

Future richer command representation.

```c id="j1n1gc"
typedef struct {
    char** args;
    char* input_file;
    char* output_file;
    int background;
} Command;
```

Supports:

* pipes
* redirection
* background jobs

---

---

# Implementation Order

Recommended order.

---

## Phase 1 (current)

* shell loop
* parser
* builtins
* raw mode
* history

---

## Phase 2

* external commands
* fork
* execvp
* waitpid

---

## Phase 3

* PATH resolution
* which

---

## Phase 4

* SQLite history database

---

## Phase 5

* provenance database

---

## Phase 6

* pipes

---

## Phase 7

* redirection

---

## Phase 8

* signals

---

# Core Execution Flow

```text id="p7i7rq"
Prompt View
   ↓
Terminal View
   ↓
Shell Controller
   ↓
Parser Service
   ↓
Command Controller
   ↓
Builtin or External Executor
   ↓
History Model
   ↓
Provenance Model
```

---

# System Design Principles

This project follows:

* Separation of concerns
* Layered architecture
* MVC-inspired design
* Explicit memory management
* Persistent storage abstraction
* Modular command execution
