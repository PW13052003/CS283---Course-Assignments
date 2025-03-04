1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

> After forking all  child processes, parent process closes the pipes which avoids file descriptor leaks. If we do not call waitpid(), the shell might immediately print the prompt without the child process having finished executing. It is also possible that the child processes remain in the process table after finishing execution.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

> The dup2() function is used to redirect standard input and standard output to pipe file descriptors in order to transferdata between piped commands. If we leave pipes open, we may never receive an EOF signal. It can also cause file descriptor leaks.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

> The cd command is implemented as a built-in rather than an external command because changing the current working directory affects the shell process itself. Unlike external commands, cd needs to modify the environment of the shell process which external processes cannot do. 

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

> To support an arbitrary number of piped commands, we need to use dynamic memory allocation instead of a static array. Wecan dynamically allocate memory based on the number of piped commands by modifying build_cmd_list() function.


