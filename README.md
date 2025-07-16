<p align="center"> 
<img src="https://img.shields.io/badge/-000000?style=for-the-badge&logo=42&logoColor=white)](https://42.fr))"> 
<img src="https://img.shields.io/badge/1337-000000?style=for-the-badge&logo=1337&logoColor=white)](https://1337.ma))">  
<img src="https://img.shields.io/badge/Language-C-blue?style=for-the-badge&logo=c)">
</p>

<h1 align="center">MiniShell</h1>
<p align="center">
  <b>A professional, robust Unix shell implemented in C for the 42 School curriculum</b>
</p>

---

## 🚀 Introduction

Welcome to **MiniShell**, a project developed as part of the rigorous 42 School curriculum. This shell is a compact, robust, and standards-compliant Unix shell, designed to mimic essential features of Bash. The implementation demonstrates advanced knowledge of process control, memory management, parsing, and system programming in C.

---

## ✨ Key Features

- **Interactive Shell Prompt**: Customizable prompt displaying current path and status.
- **Command Parsing**: Advanced lexer and parser handling complex input with quotes, escapes, and operators.
- **Built-in Commands**: Fully supports `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit` as built-ins with custom logic.
- **Pipes & Redirections**: Supports multi-level pipelines and both input/output redirections (`|`, `>`, `>>`, `<`, `<<`).
- **Environment Variable Expansion**: Handles `$VAR`, `$?`, and robust environment manipulation.
- **Signal Handling**: Graceful handling of SIGINT (Ctrl+C), SIGQUIT (Ctrl+\), SIGTERM, and EOF (Ctrl+D).
- **Error Management**: Meaningful, user-friendly error and status messages for invalid input, failed commands, and edge cases.
- **Memory Safety**: Zero memory leaks (validated with Valgrind and other tools).
- **Cross-Platform**: Works reliably on Linux and macOS.

---

## 🏗️ Architecture Overview

```
             +---------------------+
             |      User Input     |
             +----------+----------+
                        |
                        v
             +---------------------+
             |     Lexer/Parser    |
             +----------+----------+
                        |
                        v
             +---------------------+
             |   Syntax Analyzer   |
             +----------+----------+
                        |
                        v
             +---------------------+
             | Command Execution   |
             +----------+----------+
                        |
        +---------------+---------------+
        |                               |
        v                               v
+---------------+             +------------------+
| Built-in cmds |             | External binaries|
+---------------+             +------------------+
```

- **Lexer/Parser**: Tokenizes and parses input into an Abstract Syntax Tree (AST).
- **Syntax Analyzer**: Checks for syntax errors, validates redirections and pipes.
- **Executor**: Decides between built-in and external commands, manages child processes, pipes, and I/O.
- **Signal Handler**: Catches and processes signals gracefully.

---

## 📜 Supported Built-ins

| Command   | Description                                       | Example Usage                      |
|-----------|---------------------------------------------------|------------------------------------|
| `echo`    | Print arguments to standard output                | `echo Hello, 42!`                  |
| `cd`      | Change current working directory                  | `cd /usr/local`                    |
| `pwd`     | Print current working directory                   | `pwd`                              |
| `export`  | Set or display environment variables              | `export PATH=/bin`                 |
| `unset`   | Remove environment variable                       | `unset PATH`                       |
| `env`     | Display environment variables                     | `env`                              |
| `exit`    | Exit the shell                                    | `exit [status]`                    |

---

## 🔍 Advanced Features

- **Heredoc (`<<`)**: Reads multiline input until a delimiter is reached.
- **Quote Handling**: Supports single `'`, double `"`, and escaped characters.
- **Return Status**: Updates `$?` variable after each command.
- **Executable Search**: Resolves binaries using `PATH`, with custom error messages if not found.
- **Robust Error Handling**: Detects and reports syntax errors, invalid file descriptors, and permission issues.

---

## 🧠 Learning Outcomes

- Deep understanding of Unix process management (`fork`, `execve`, `pipe`, `dup2`, `waitpid`).
- Advanced parsing and tokenization techniques in C.
- Signal programming and asynchronous event handling.
- Robustness and safety in C programming (resource management, error handling).

---

## 📚 Resources

### 📺 Videos & Playlists

- [Build Your Own Shell - Full Playlist (YouTube, CodeVault)](https://www.youtube.com/playlist?list=PLfqABt5AS4FkW5mOn2Tn9ZZLLDwA3kZUY)
- [How to build your own shell in C (YouTube, CodeVault)](https://www.youtube.com/watch?v=BI3K-ME3L74)
- [Writing a Simple Shell in C (YouTube, CodeVault)](https://www.youtube.com/watch?v=yTR00r8vBH8)

### 📝 Blog Posts & Guides

- [Writing a Shell (danishpraka.sh)](https://danishpraka.sh/posts/write-a-shell/)
- [Writing a Unix Shell - Part 1 (Ishaan Gupta)](https://igupta.in/blog/writing-a-unix-shell-part-1/)

### 🛠️ Open Source Repositories & Workshops

- [tokenrove/build-your-own-shell (GitHub)](https://github.com/tokenrove/build-your-own-shell)
- [kamalmarhubi/shell-workshop (GitHub)](https://github.com/kamalmarhubi/shell-workshop)

...etc

---

## 📄 License

This project is for educational purposes as part of the 42 School curriculum and is not intended for production use.

---

<p align="center">
  <img src="https://img.shields.io/badge/42-Network-000000?style=flat-square&logo=42&logoColor=white">
</p>
