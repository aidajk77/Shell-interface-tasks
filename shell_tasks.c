#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>


#define MAX_ARGS 64
#define DELIM " \t\r\n\a"

void rm_command(char *filename) {
    struct stat file_stat;
    if (stat(filename, &file_stat) == -1) {
        perror("rm");
        return;
    }

    if (S_ISDIR(file_stat.st_mode)) {
        // Directory: Use rmdir or remove recursively
        if (rmdir(filename) == -1) {
            perror("rm: Failed to remove directory");
        }
    } else {
        // File: Use unlink to delete the file
        if (unlink(filename) == -1) {
            perror("rm: Failed to remove file");
        }
    }
}

void cat_command(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("cat");
        return;
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }
    fclose(file);
}

void clear_command() {
    // For Unix-like systems, the clear command is typically implemented as:
    system("clear");
}


void cowsay_command(char *message) {
    // Simple cow ASCII art
    printf(" _________  \n");
    printf("< %s >\n", message);
    printf(" ---------  \n");
    printf("   \\   ^__^ \n");
    printf("    \\  (oo)\\_______\n");
    printf("       (__)\\       )\\/\\\n");
    printf("           ||----w |\n");
    printf("           ||     ||\n");
}

void cat_command_with_options(char *filename, int show_line_numbers) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("cat");
        return;
    }

    char line[1024];
    int line_number = 1;
    while (fgets(line, sizeof(line), file)) {
        if (show_line_numbers) {
            printf("%d: %s", line_number++, line);
        } else {
            printf("%s", line);
        }
    }
    fclose(file);
}

void rm_recursive(char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("rm");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

            struct stat entry_stat;
            if (stat(full_path, &entry_stat) == 0) {
                if (S_ISDIR(entry_stat.st_mode)) {
                    rm_recursive(full_path);  // Recursively remove directory contents
                    rmdir(full_path);         // Remove the empty directory
                } else {
                    unlink(full_path);        // Remove the file
                }
            }
        }
    }
    closedir(dir);
}

void redirect_output(char *command, char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("fopen");
        return;
    }

    // Redirect the output of the command to the file
    fprintf(file, "%s\n", command);  // In a real shell, run the command here instead
    fclose(file);
}



void display_prompt() {
    char hostname[HOST_NAME_MAX + 1];
    char *username = getenv("USER");

    if (gethostname(hostname, sizeof(hostname)) != 0) {
        perror("gethostname");
        strcpy(hostname, "unknown");
    }

    if (!username) {
        username = "user";
    }

    printf("\033[31m%s@%s:~$ \033[0m", hostname, username);
}

char *read_input() {
    char *input = malloc(MAX_INPUT);
    if (!input) {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }

    if (fgets(input, MAX_INPUT, stdin) == NULL) {
        printf("\n");
        exit(EXIT_SUCCESS);  // Exit gracefully on Ctrl+D
    }

    return input;
}

char **parse_input(char *input) {
    int bufsize = MAX_ARGS;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(input, DELIM);
    while (token != NULL) {
        tokens[position++] = token;

        if (position >= bufsize) {
            bufsize += MAX_ARGS;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

void execute_piped_command(char *input) {
    char *cmd1 = strtok(input, "|");
    char *cmd2 = strtok(NULL, "|");

    if (!cmd1 || !cmd2) {
        fprintf(stderr, "Invalid piped command\n");
        return;
    }

    // Parse the commands into arguments
    char **args1 = parse_input(cmd1);
    char **args2 = parse_input(cmd2);


    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Pipe failed");
        return;
    }

    pid_t p1 = fork();
    if (p1 == -1) {
        perror("Fork failed for first child");
        return;
    }

    if (p1 == 0) {
        // First child (left side of the pipe)
        close(pipefd[0]); // Close the read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe write
        close(pipefd[1]); // Close after dup2

        execvp(args1[0], args1);
        perror("execvp 1 failed");
        exit(EXIT_FAILURE);
    }

    pid_t p2 = fork();
    if (p2 == -1) {
        perror("Fork failed for second child");
        return;
    }

    if (p2 == 0) {
        // Second child (right side of the pipe)
        close(pipefd[1]); // Close the write end
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe read
        close(pipefd[0]); // Close after dup2

        execvp(args2[0], args2);
        perror("execvp 2 failed");
        exit(EXIT_FAILURE);
    }

    // Parent process
    close(pipefd[0]);
    close(pipefd[1]);

    // Wait for child processes to finish
    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);

    // No need to free args1 and args2 here as execvp replaces the current process
}


int execute_command(char **args) {

    if (args[0] == NULL) {
        return 1; // Empty command
    }
// Reconstruct the input string to check for pipe
    char input_str[1024] = "";
    for (int i = 0; args[i] != NULL; i++) {
        strcat(input_str, args[i]);
        strcat(input_str, " ");
    }
    if (strchr(input_str, '|')) {

        execute_piped_command(input_str);
        return 1;
    }


    if (strcmp(args[0], "exit") == 0) {
        return 0;
    } else if (strcmp(args[0], "clear2") == 0) {
        clear_command();
        return 1;
    } else if (strcmp(args[0], "cat2") == 0) {
        if (args[1] != NULL && strcmp(args[1], "-n") == 0 && args[2] != NULL) {
            cat_command_with_options(args[2], 1); // With line numbers
        } else if (args[1] != NULL) {
            cat_command_with_options(args[1], 0); // Normal
        } else {
            fprintf(stderr, "cat: missing file name\n");
        }
        return 1;
    } else if (strcmp(args[0], "rm2") == 0) {
        if (args[1] != NULL && strcmp(args[1], "-r") == 0 && args[2] != NULL) {
            rm_recursive(args[2]);  // Recursive removal
        } else if (args[1] != NULL) {
            rm_command(args[1]);  // Regular file
        } else {
            fprintf(stderr, "rm: missing operand\n");
        }
        return 1;
    } else if (strcmp(args[0], "cowsay2") == 0) {
        if (args[1] != NULL) {
            cowsay_command(args[1]); // Say the message
        } else {
            printf("cowsay: missing message\n");
        }
        return 1;
    }

    // Not a built-in command → execute as external command
    pid_t pid = fork();
    if (pid == 0) {
        // In child
        if (execvp(args[0], args) == -1) {
            perror("execvp");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork");
    } else {
        // In parent
        wait(NULL);
    }
    return 1;
}


void shell_loop() {
    char *input;
    char **args;
    int status;

    do {
        display_prompt();
        input = read_input();
        args = parse_input(input);
        status = execute_command(args);

        free(input);
        free(args);
    } while (status);
}

int main() {
    shell_loop();
    return 0;
}
