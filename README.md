# POSIX Shell

A modular, interactive shell implemented in C++, designed to mimic basic functionality of Unix shells. This shell supports built-in commands, process management, I/O redirection, pipelines, background execution, and more.

---

## How to Compile

Run the following command in the project directory:

```bash
make
```

---

## How to Run

After compiling, run:

```bash
./run.out
```

---

## How to Exit

Type:

```bash
exit
```

or press `Ctrl+D`.

---

## Commands Implemented

1. Basic shell commands: `cd`, `echo`, `pwd`
2. `ls` command (with options)
3. System commands (foreground and background, with/without arguments)
4. `pinfo` – process info display
5. `search` – file search utility
6. Input/output redirection (`<`, `>`)
7. Command piping (`|`)
8. Simple signal handling
9. Command history:

   * `history <num>` – shows last *num* commands
  
