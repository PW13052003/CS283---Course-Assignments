1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() is a good approach because it allows us to specify a buffer size which ensures we don't read beyond the allocated memory. The fgets() function can also handle line based input until a newline or end of file is reached. 

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  Using malloc() to allocate memory for cmd_buff is useful because a dynamically allocated buffer allows us to tweak its size based on the memory that is actually required. This prevents memory wastage.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  If we don't trim spaces, the shell might interpret them as arguments and cause error. 

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  I referred to this website to understand the concepts needed in this problem: https://www.howtogeek.com/435903/what-are-stdin-stdout-and-stderr-on-linux/ . We can redirect STDOUT to a file (ls > output.txt). The challenge is that we need to open the file first, and then redirection happens. We can also append to a file echo "Add output" >> output.txt. It is important we open the file in append mode because if we don't, the file data may be erased. Finally, we can redirect STDERR to a file. There isn't any particular challenge to this one. 

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection 'redirects' output to files or input from files whereas piping sends one command's output to another command's input. This means that redirection typically works with files whereas piping typically works with processes.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  Keeping STDERR and STDOUT separate helps us debug easily in case there are issues with the code because we do not need to touch the part of code that is correct. Also, given we want to store the output in a file, we can still print the error to the console which maintains a bit of exclusivity in terms of what is printed where. 

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  I think we don't require to merge them but it is possible to do so by redirecting STDERR to STDOUT. Check exit status, allow 2>. 


