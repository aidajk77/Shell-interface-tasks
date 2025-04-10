README - Shell Project
======================

Group Members:
--------------
- Tarik Topić
- Aid Ajkunić

Submission Contents:
--------------------
- shell
- shell.c
- task3
- task3.c

Assignment Questions:
---------------------
1.5.1 mmap() function is used in C to map files/devices into memory. Instead of reading or writing data from a file using regular I/O functions, you can directly access the file's content as if it was part of the program's memory.it creates a region in the process's memory that is linked to the file or device. This allows you to read from and write to the file just like you would with any array in memory. It is an efficient way to deal with large amounts of data.
1.5.2 A context switch happens when the OS saves the state of the currently running process and loads the state of another process to allow multitasking.
A context switch happens when the OS saves the current process's state, like the CPU registers and program counter, so it can continue later from the same point. Then, the OS loads the state of the next process, including its registers and program counter, so that it can keep running like it was never interrupted. This process allows the OS to switch between processes quickly, making it look like multiple processes are running at the same time.
The number of context switches depends on the situation. Generally, we want fewer context switches because each one takes time and resources, and too many can slow down performance by reducing the time spent on actual tasks. However, in certain cases, like real-time systems or when high multitasking is required, more context switches may be necessary to give the impression of parallel execution, even on a single-core processor.
1.5.3 the output is : 
child
parent
Becouse execv() failes the "child"  is printed and then the parent waits (wait()) for the child to finish and prints "parent".

What We Did:
------------
In this project, we developed a custom shell interface that supports the execution of external programs and a number of built-in functions such as `cd`, `exit`, `rm`, and more.

We focused on understanding how processes, file systems, and command parsing work in a Unix-like environment. Our shell supports:

- Executing basic commands (e.g., `ls`, `echo`, `pwd`)
- Removing files and directories (`rm`, including a recursive version)
- Handling command input and output with a custom prompt
- Splitting responsibilities into multiple files for modularity (`shell.c`, `task3.c`)

Compilation Instructions:
-------------------------
To compile the project, simply run:
gcc -o shell shell.c gcc -o task3 task3.c


Make sure you have `gcc` installed and are using a Unix-like environment (Linux/macOS or WSL).

Challenges Encountered:
------------------------
One major challenge we encountered was implementing the `grep` instruction. While other commands such as `rm` and `cd` worked as expected, `grep` does not currently function properly. We suspect this may be related to how we handle piping or input arguments, and with more time we would investigate how to better manage `execvp()` usage when dealing with commands that require input files or stdin.

Sources & References:
---------------------
- ChatGPT (used for explanations and examples)
- Stack Overflow
- YouTube tutorials on shell implementation
- Linux `man` pages (e.g., `man stat`, `man execvp`)
- Various documentation and online blogs

Known Issues:
-------------
- `grep` command does not currently work. We believe this may be due to incorrect handling of its arguments or how the command expects input. With more time, we would isolate the input parsing or look into how to implement piping/redirection properly for such commands.

Code Comments:
--------------
Our code is fully commented to describe the behavior of each function, important logic flows, and command-handling logic. We prioritized readability and maintainability.


