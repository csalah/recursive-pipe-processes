# Recursive Pipe and Process Creation in C

This C program demonstrates recursive process creation and inter process communication using pipes. Each process spawns a child, passing a decremented process number down the chain. The child process sends messages through a pipe which the parent reads and prints to standard output.

---

## 🔧 How It Works

- The program accepts a **single integer argument** representing the number of processes to create.
- Each process:
  - Displays a message when it starts and finishes
  - Creates a child (unless it's the last one)
  - Reads messages from the pipe connected to its child
- The parent waits for the child to finish before printing its own completion message
- The program uses `fork()`, `pipe()`, `execlp()`, `dup2()`, `read()`, `write()`, and `wait()` system calls

---

## ▶️ Compilation & Usage

```bash
gcc -o cpr cpr.c
./cpr <number_of_processes>
