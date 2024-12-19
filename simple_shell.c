#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

/**
 * main - main function
 *
 * Return: Always 0.
 */
int main(void)
{
	while (1)
	{
		char **argv = NULL;
		char *line = NULL;
		int status;

		printf("$ ");

		argv = _get_line(&line);
		if (argv == NULL)
		{
			free(line);
			return (-1);
		}

		if (_strcmp(argv[0], "exit") == 0)
		{
			free(argv);
			free(line);
			break;
		}

		get_command(argv);

		wait(&status);
		_wait_status(status);

		free(argv);
		free(line);
	}

	return (0);
}

/**
 * _wait_status - get status of the wait function
 * @status: the return status of the child process
 * Return: int
 */
int _wait_status(int status)
{
	if (status == -1)
	{
		perror("Error waiting child");
		return (-1);
	}

	return (0);
}

/**
 * get_command - find the format of the command, then execve it
 * @argv: the command to execute
 * Return: int
 */
int get_command(char **argv)
{
	pid_t pid;

	if (argv[0][0] != '/')
	{
		char *_path = "/bin/";
		char buffer[100];

		argv[0] = realloc(argv[0], _strlen(_path) + _strlen(argv[0]) + 1);
		if (argv[0] == NULL)
			return (-1);

		_strcpy(buffer, argv[0]);
		_strcpy(argv[0], _path);
		_strcat(argv[0], buffer);
	}

	pid = fork();
	if (pid == -1)
	{
		perror("Error Fork");
		exit(-1);
	}

	if (pid == 0)
		execve(argv[0], argv, NULL);

	return (0);
}

/**
 * _get_line - get the line (stdin) and split it into tokens
 * @line: the str to tokenize
 * Return: char **
 */
char **_get_line(char **line)
{
	int i = 0;
	size_t len = 0;
	size_t bytes_read;
	char *token = NULL;
	char **array = NULL;
	size_t capacity = 10;

	bytes_read = getline(line, &len, stdin);
	if (bytes_read > 0 && (*line)[bytes_read - 1] == '\n')
		(*line)[bytes_read - 1] = '\0';
	array = malloc(sizeof(char *) * capacity);
	if (array == NULL)
	{
		perror("Malloc failed");
		exit(1);
	}
	token = strtok(*line, " ");
	while (token != NULL)
	{
		if (i >= capacity)
		{
			capacity *= 2;
			char **new_array = malloc(sizeof(char *) * capacity);

			if (new_array == NULL)
			{
				perror("Malloc failed");
				exit(1);
			}
			for (int j = 0; j < i; j++)
				new_array[j] = array[j];
			free(array);
			array = new_array;
		}
		array[i] = token;
		i++;
		token = strtok(NULL, " ");
	}
	array[i] = NULL;
	return (array);
}