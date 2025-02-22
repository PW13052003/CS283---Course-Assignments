1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  If we use execvp directly, it would essentially overwrite the shell and once the command finishes executing, we will not be able to return to out original shell.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If fork() system call fails, it means no new child process is created. I print an error message to stderr (using perror) if the fork() fails. Without handling this scenario properly, the parent process might assume that the child is running properly and will also run normally.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  The execvp() function looks for a specified command. If that command is an absolute or relative path, execvp() uses it directly. If not, it looks into every directory in the PATH environment variable and tries to execute the first matching executable file it finds. If no file is found, it displays an error. So its the PATH environment variable.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  The wait() function ensures that the parent process doesn't execute until the child process is completed. If we don't call wait(), child process will exit but still occupy a PID which can lead to system's process table overflow which obviously means an increased likelihood of slowdowns and inability to launch new programs. I have used waitpid() though which also does the job.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXITSTATUS() is used to extract the exit status of a child process after wait() has been called. It is important because it can tell us what kind of error has occured making debugging slightly easier.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  My build_cmd_buff() function first detects if an argument starts with a quote (either double or single) which ensures we identify quoted arguments early on. Then I store the argument position and remove the opening quote. My function then finds the closing quote and null terminates it. Finally, arguments without quotes are tokenized based on spaces. My approach is necessary because we need to preserve multi word arguments. For instance, "hello world" is one argument not two. 

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  In part 1, I split commands using pipes and treated them as separate commands whereas in part 2, I have not used pipe handling. In part 1, I used strtok_r() to extract command name and arguments, handling them as separate tokens whereas I have not done that here. In part 1, I was building a structured command list where each command had a separate command and argument whereas I am now using only one command per execution with argc, argv[] and _cmd_buffer to manage input. The refactoring part was definitely challenging, I did not think I would have to make these many adjustments and got confused initially. For instance, I kept trying to use strtok_r() to preserve spaces inside quotes. However, after getting advice, I decided to stop trying to use strtok_r() because it wasn't able to do what I wanted to do. I ended up switching to manually looping through the string, detecting quotes and preserving the spaces properly. 

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals can enable process control and debugging, notify a process of external events and also handle unexpected events. Signals can interrupt a process at any time whereas other IPC methods must actively read/write data. Signals are just an interrupt with signal numbers whereas IPCs are fairly complex. Signals just have a number, they don't really carry data.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL (signal number 9) immediately terminates the process. This should only be used when a process needs to be force stopped or is unresponsive. SIGTERM (signal number 15) also terminates the process but this doesn't happen immediately and allows a program to perform a cleanup before exiting. SIGINT (signal number 2) also terminates the process and one of the most common examples is using Ctrl+C to terminate an ongoing process.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  When a process receives SIGSTOP, it pauses the process execution. This doesn't mean termination, the process still remains in memory but just doesn't follow further steps. It cannot be ignored like SIGNINT.




