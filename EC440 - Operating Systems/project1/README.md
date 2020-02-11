
## METHODOLOGY

I decided the easiest way to go about this would be to:

1. Prompt user for input
2. Take the input line from the user as a string
3. Convert this string into an array of tokens, including all meta-characters
4. Iterating through this array of tokens, and doing the following based on the current token: 
	* If the current token is not a meta-character (it is just a normal string), add it to the args list for the current command
	* If the current token is a < or >, save the file name that is stored in the next token
	* If the current token is a &, make sure will run the current command in the background
	* If the current token is either a |, or the end of the array, we execute the current command. If the current token is |, we obviously also need to set up the pipe for into the next command. Also, if we previously hit a < or >, obviously change STDIN or STDOUT to the correct file.
5. Repeat from step 1 until CTRL+D is inputted

This may not be the best or most straight forward way to go about this task, but it worked well for me. At least better than the other ways I was trying. 

## Complications

At first I was doing the above without converting the string into an array of tokens, instead converting the string straight into the args list. This worked well when I was implementing the file redirects, but as soon as I started implementing piping, this was an issue since I could not execute multiple commands in one line. So I switched to the described methodology, and it worked a lot better.

It also took me a while to figure out how to implement the piping correctly, as I was kind of confused how they work when you weren't simply communicating between a parent and child, but also storing the output of a child/command for a later child/command. 

It also took me a while to figure out how to deal with background processes. Initially I set up my signal handler as the following: signal(SIGCHLD, SIG_IGN). However, since this does not allow me to get the status from a child using wait(), I could not thow an error message when an invalid command was inputted. I then created my own signal handler function, which contains waitpid(-1, NULL, WNOHANG). I initially did not include the WNOHANG flag, which caused the shell to stop working if you input a regular command while a different background process was running, but WNOHANG thankfully fixed this.