#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

#define TRUE 1
#define FALSE 0
#define MAX_LINE 512
#define MAX_TOKEN 32

_Bool isSpecialCharacter(char c)
{
  return (c == ' ' || c == '|' || c == '&' || c == '<' || c == '>' || c == '\n');
}

// Signal handler which runs when a child process finishes
void sigHandler(){
  waitpid(-1, NULL, WNOHANG);
}

int main(int argc, char **argv)
{
  // To store input line, tokens in that line, and args lists for each command
  char command[MAX_LINE];
  char *tokens[MAX_TOKEN];
  char *args[MAX_TOKEN];

  // To keep track of which meta-chars are being used
  _Bool redirectInput = FALSE;
  _Bool redirectOutput = FALSE;
  _Bool runInBackground = FALSE;
  _Bool piping = FALSE;
  _Bool isPipe = FALSE;


  // To keep track of input/output file names
  char *inputFile;
  char *outputFile;

  // For all file descriptors that may be used
  int input_fd;
  int output_fd;
  int stdin_copy = dup(0);
  int stdout_copy = dup(1);
  int pipefd[2];
  int pipefd_res;

  // Loop variable
  int i;

  int wstatus, wpid;

  // Check if -s arg was used, if so suppress shell prompt 'my_shell$'
  _Bool showPrompt = TRUE;
  if (argc > 1 && strcmp(argv[1], "-n") == 0)
  {
    showPrompt = FALSE;
  }

  // Signal handler to collect pid's of background processes
  signal(SIGCHLD, sigHandler);

  // Main loop
  while (TRUE)
  {
    // Show (or don't) shell prompt
    if (showPrompt)
    {
      write(1, "my_shell$ ", 11);
    }

    // Get the command from the user. If fgets returns null, then user entered CTRL+D, so exit myshell
    if (fgets(command, MAX_LINE, stdin) == NULL)
    {
      while ((wpid = wait(NULL)) > 0);
      return 0;
    }

    // Variables for converting from command string to an array of tokens
    char *token = (char *)malloc(MAX_TOKEN * sizeof(char));
    int tokenNum = 0;
    int previous = 0;

    // Convert command string into an array of tokens
    for (i = 0; i < strlen(command); i++)
    {
      // If the current char is not a meta-char, add it to the current token
      if (!isSpecialCharacter(command[i]))
      {
        *(token + i - previous) = command[i];
      }
      // If the current char is a meta-char or space/newline, finish off the current token and add it to the array
      else
      {
        *(token + i - previous) = '\0';
        // Ignore empty tokens (this will happen with multiple spaces in row)
        if (strcmp(token, "") != 0)
        {
          // If the token is not empty, add it to the array
          tokens[tokenNum] = (char *)malloc(MAX_TOKEN * sizeof(char));
          strcpy(tokens[tokenNum], token);
          tokenNum++;
        }
        // Reallocate token string to a new memory address
        free(token);
        token = (char *)malloc(MAX_TOKEN * sizeof(char));

        // If the curent char is any of the meta-chars, add the meta-char as its own token in the array
        if (command[i] == '<' || command[i] == '>' || command[i] == '|' || command[i] == '&')
        {
          *(token) = command[i];
          *(token + 1) = '\0';
          tokens[tokenNum] = (char *)malloc(MAX_TOKEN * sizeof(char));
          strcpy(tokens[tokenNum], token);
          tokenNum++;
          free(token);
          token = (char *)malloc(MAX_TOKEN * sizeof(char));
          if(command[i] == '&'){
            runInBackground = TRUE;
          }
        }
        previous = i + 1;
      }
    }
    // Add NULL pointer to end of token array so we know where the array ends
    tokens[tokenNum] = NULL;
    free(token);
    if(tokenNum == 0){
      continue;
    }

    // Parse through the token array, executing all the commands
    int argNum = 0;
    for (i = 0; i <= tokenNum; i++)
    {

      // If we are not at the end of an array, nor have we reached a pipe, we are not ready to execute yet
      if (tokens[i] != NULL && strcmp(tokens[i], "|") != 0)
      {

        // If the current token is "<", save the input file name
        if (strcmp(tokens[i], "<") == 0)
        {
          // Check for multiple "<" chars
          if (redirectInput || strcmp(tokens[i + 1], "<") == 0)
          {
            write(1, "ERROR:Only one input redirect is allowed per command\n", 54);
            redirectInput = FALSE;
            break;
          }
          // If piping is true, we are no longer in the first command, so we cannot have "<"
          if (piping)
          {
            write(1, "ERROR:Only the first command can have its input redirected\n", 60);
            piping = FALSE;
            break;
          }

          // Save the input file name
          inputFile = (char *)malloc(MAX_TOKEN * sizeof(char));
          strcpy(inputFile, tokens[i + 1]);
          redirectInput = TRUE;
          i++;
        }

        // If the current token is ">", save the output file name
        else if (strcmp(tokens[i], ">") == 0)
        {
          // check for multiple ">" chars
          if (redirectOutput || strcmp(tokens[i + 1], ">") == 0)
          {
            write(1, "ERROR:Only one output redirect is allowed per command\n", 55);
            redirectOutput = FALSE;
            break;
          }

          // Save the output file name
          outputFile = (char *)malloc(MAX_TOKEN * sizeof(char));
          strcpy(outputFile, tokens[i + 1]);
          redirectOutput = TRUE;
          i++;
        }

        // If the current token is "&", make sure we execute the line in the background
        else if (strcmp(tokens[i], "&") == 0)
        {
          // If the next token in the aray is not null, the "&" char is not at the end of the line
          if (tokens[i + 1] != NULL)
          {
            write(1, "ERROR:& can only appear at the end of the line\n", 48);
            break;
          }
          runInBackground = TRUE;
        }

        // If none of these meta-chars were hit, add the current token to the args list
        else
        {
          args[argNum] = (char *)malloc(MAX_TOKEN * sizeof(char));
          strcpy(args[argNum], tokens[i]);
          argNum++;
        }
      }

      // If we get here, then the current token is either the end of the array, or a pipe, which means it's time to execute a command
      else
      {
        // Add null pointer to the end of the args array
        args[argNum] = NULL;

        // If current token is not NULL, then current token is a pipe ("|")
        isPipe = FALSE;
        if (tokens[i] != NULL)
        {
          isPipe = TRUE;
        }

        // If we are preparing to pipe
        if (isPipe)
        {

          // If we are at a pipe, and we are already redirecting output, then a ">" char is in a command other than the last one
          if (redirectOutput)
          {
            write(1, "ERROR:Only the last command can have its output redirected\n", 60);
            redirectOutput = FALSE;
            break;
          }

          // Create the pipe
          if (pipe(pipefd))
          {
            write(1, "ERROR:Pipe error\n", 18);
          }
        }

        // Fork to execute
        pid_t pid = fork();
        int status;

        // Child
        if (pid == 0)
        {
          // If we are redirecting input, open the input file and redirect it into STDIN
          if (redirectInput)
          {
            input_fd = open(inputFile, O_RDONLY | O_APPEND);
            if (input_fd < 0)
            {
              write(1, "ERROR:Input file not found\n", 28);
              redirectInput = FALSE;
              free(inputFile);
              break;
            }
            close(0);
            dup2(input_fd, 0);
            close(input_fd);
          }

          // If we are redirecting output, open the output file and redirect it into STDOUT
          if (redirectOutput)
          {
            output_fd = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
            close(1);
            dup2(output_fd, 1);
            close(output_fd);
          }

          // If the current token is a pipe, set STDOUT to the write side of the pipe
          if (isPipe)
          {
            close(1);
            dup2(pipefd[1], 1);
            close(pipefd[1]);
          }

          // If the current command is coming after a pipe, set STDIN to the read side of the previous pipe
          if (piping)
          {
            close(0);
            dup2(pipefd_res, 0);
            close(pipefd_res);
          }

          // Execute the command
          status = execvp(args[0], args);

          // Reset STDIN and STDOUT
          dup2(stdout_copy, 1);
          dup2(stdin_copy, 0);

          // Exit the child
          exit(status);
        }
        // Parent
        else if (pid > 0)
        {

          // Wait for the child to finish, unless we are running in the background
          if (!runInBackground)
          {
            pid = waitpid(pid, &wstatus, 0);

            if(wstatus == 65280){
              write(1, "ERROR:Invalid command\n", 23);
              break;
            }
          }

          // If we are piping after the current command, save the file descriptor of the read side of the current pipe
          if (isPipe)
          {
            pipefd_res = dup(pipefd[0]);

            // Close the pipe
            close(pipefd[0]);
            close(pipefd[1]);
          }
        }
        // Error in forking
        else
        {
          write(1, "ERROR:fork() error\n", 20);
        }

        // If we are piping after the current command, set piping to true so we remember to set STDIN as this pipe
        if (isPipe)
        {
          piping = TRUE;
        }

        // Reset all necessary variables after the current command is processed
        free(args[0]);
        argNum = 0;
        if (redirectInput)
        {
          free(inputFile);
          redirectInput = FALSE;
        }
        if (redirectOutput)
        {
          free(outputFile);
          redirectOutput = FALSE;
        }
      }
    }

    // Reset the rest of the variables after all of the commands in the current line are processed
    // Reset all necessary variables after the current command is processed
    // free(args[0]);
    argNum = 0;
    // Reset STDIN and STDOUT
    dup2(stdout_copy, 1);
    dup2(stdin_copy, 0);

    piping = FALSE;
    runInBackground = FALSE;
  }
}
