#include "shell.h"

int proc_file_commands(char *file_path, int *exe_ret);
int cant_open(char *file_path);

/**
 * proc_file_commands - run commands stored in a file
 * @file_path: Path to the file.
 * @exe_ret: Return value of the last executed command.
 *
 * Return: If file couldn't be opened - 127.
 *	   If malloc fails - -1.
 *	   Else the return value of the last command ran.
 */

int proc_file_commands(char *file_path, int *exe_ret)
{

	ssize_t file, b_read, index;
	int ret;
	unsigned int old_size = 120;
	unsigned int line_size = 0;
	char *line, **args, **front;
	char buffer[120];

	hist = 0;
	file = open(file_path, O_RDONLY);
	if (file == -1)
	{
		*exe_ret = cant_open(file_path);
		return (*exe_ret);
	}
	line = malloc(sizeof(char) * old_size);
	if (!line)
		return (-1);
	do {
		b_read = read(file, buffer, 119);
		if (b_read == 0 && line_size == 0)
			return (*exe_ret);
		buffer[b_read] = '\0';
		line_size += b_read;
		line = _realloc(line, old_size, line_size);
		_strcat(line, buffer);
		old_size = line_size;
	} while (b_read);
	for (index = 0; line[index] == '\n'; index++)
		line[index] = ' ';
	for (; index < line_size; index++)
	{
		if (line[index] == '\n')
		{
			line[index] = ';';
			for (index += 1; index < line_size && line[index] == '\n'; index++)
				line[index] = ' ';
		}
	}
	variable_replacement(&line, exe_ret);
	handle_line(&line, line_size);
	args = _strtok(line, " ");
	free(line);
	if (!args)
		return (0);
	if (check_args(args) != 0)
	{
		*exe_ret = 2;
		free_args(args, args);
		return (*exe_ret);
	}
	front = args;

	for (index = 0; args[index]; index++)
	{
		if (_strncmp(args[index], ";", 1) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			ret = call_args(args, front, exe_ret);
			args = &args[++index];
			index = 0;
		}
	}

	ret = call_args(args, front, exe_ret);

	free(front);
	return (ret);
}


/**
 * cant_open - Print a cant open error if the file doesn't exist or
 * lacks proper permissions
 * @file_path: Path to the supposed file.
 *
 * Return: 127.
 */

int cant_open(char *file_path)
{
	char *error, *hist_str;
	int length;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (127);

	length = _strlen(name) + _strlen(hist_str) + _strlen(file_path) + 16;
	error = malloc(sizeof(char) * (length + 1));
	if (!error)
	{
		free(hist_str);
		return (127);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": Can't open ");
	_strcat(error, file_path);
	_strcat(error, "\n");

	free(hist_str);
	write(STDERR_FILENO, error, length);
	free(error);
	return (127);
}
