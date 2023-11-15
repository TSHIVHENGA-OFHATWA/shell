#include <stdioh>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10

void read_line(char *buffer, size_t size) 
{
	if (fgets(buffer, size, stdin) == NULL) 
	{
		perror("Error reading input");
		exit(EXIT_FAILURE);
	}
}

void parse_command(char *command, char **arguments)
{
	char *token = strtok(command, " \n");
	int i = 0;
	
	while (token != NULL && i < MAX_ARGUMENTS - 1) 
	{
		arguments[i++] = token;
		token = strtok(NULL, " \n");
	}
	arguments[i] = NULL;
}

void execute_command(char **arguments) 
{
	pid_t pid = fork();
	
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) 
	{
		if (execvp(arguments[0], arguments) == -1)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}
	else 
	{
		int status;
		waitpid(pid, &status, 0);

		if (WIFEXITED(status))
		{
			printf("Child process exited with status %d\n", WEXITSTATUS(status));
		}
		else
		{
			printf("Child process did not exit normally\n");
		}
	}
}

/**
 * main - main function
 */

int main(void) 
{
	char command[MAX_COMMAND_LENGTH];
	char *arguments[MAX_ARGUMENTS];
	
	while (1) 
	{
		printf("MyShell> ");
		read_line(command, sizeof(command));

		if (strcmp(command, "exit\n") == 0) 
		{
			printf("Exiting shell...\n");
			break;
		}
		
		parse_command(command, arguments);
		
		if (arguments[0] != NULL) 
		{
			execute_command(arguments);
		}
	}
	
	return (0);
}

