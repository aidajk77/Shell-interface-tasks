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
(Please fill in your answers to the provided assignment questions below.)

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


