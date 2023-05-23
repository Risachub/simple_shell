#include "shell."


int check_args(char **args);
int run_args(char **args, char **front, int *exe_ret);
int handle_args(int *exe_ret);
int call_args(char **args, char **front, int *exe_ret);
char *get_args(char *line, int *exe_ret);


/**
 * check_args - This will check if there are
 * any leading ';', ';;', '&&', or '||'.
 * @args: this is the 2D pointer to tokenize
 * commands and arguments.
 *
 * Return: If a ';', '&&', or '||' is placed at an invalid position - 2.
 *	   Otherwise - 0.
 */

int check_args(char **args)
{
	size_t i;
	char *current, *next;

	for (i = 0; args[i]; i++)
	{
		current = args[i];
		if (current[0] == ';' || current[0] == '&' || curentr[0] == '|')
		{
			if (i == 0 || current[1] == ';')
				return (create_error(&args[i], 2));
			next = args[i + 1];
			if (next && (next[0] == ';' || next[0] == '&' || next[0] == '|'))
				return (create_error(&args[i + 1], 2));
		}
	}
	return (0);
}

/**
 * run_args - This will call the execution of a command.
 * @args: array of arguments.
 * @front: This is a double pointer to the beginning of args.
 * @exe_ret: This is the return value of the
 * parent process' last executed command.
 *
 * Return: return value of the last executed command.
 */

int run_args(char **args, char **front, int *exe_ret)
{
	int ret, i;
	int (*builtin)(char **args, char **front);

	builtin = get_builtin(args[0]);

	if (builtin)
	{
		ret = builtin(args + 1, front);
		if (ret != EXIT)
			*exe_ret = ret;
	}
	else
	{
		*exe_ret = execute(args, front);
		ret = *exe_ret;
	}

	hist++;

	for (i = 0; args[i]; i++)
		free(args[i]);

	return (ret);
}

/**
 * handle_args - This gets, calls, and runs execution of a command.
 * @exe_ret: return value of the parent process last executed command.
 *
 * Return: If an end-of-file is read - END_OF_FILE (-2).
 *         If the input cannot be tokenized - -1.
 *         O/w - The exit value of the last executed command.
 */
int handle_args(int *exe_ret)
{
	int ret = 0, index;
	char **args, *line = NULL, **front;

	line = get_args(line, exe_ret);
	if (!line)
		return (END_OF_FILE);

	args = _strtok(line, " ");
	free(line);
	if (!args)
		return (ret);
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
	if (args)
		ret = call_args(args, front, exe_ret);

	free(front);
	return (ret);
}


/**
 * call_args - This partitions operators from commands and calls them.
 * @args: array of arguments.
 * @front: This is a double pointer to the beginning of args.
 * @exe_ret: return value of the parent process last executed command.
 *
 * Return: return the value of the last executed command.
 */

int call_args(char **args, char **front, int *exe_ret)
{
	int ret, index;

	if (!args[0])
		return (*exe_ret);
	for (index = 0; args[index]; index++)
	{
		if (_strncmp(args[index], "||", 2) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			args = replace_aliases(args);
			ret = run_args(args, front, exe_ret);
			if (*exe_ret != 0)
			{
				args = &args[++index];
				index = 0;
			}
			else
			{
				for (index++; args[index]; index++)
					free(args[index]);
				return (ret);
			}
		}
		else if (_strncmp(args[index], "&&", 2) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			args = replace_aliases(args);
			ret = run_args(args, front, exe_ret);
			if (*exe_ret == 0)
			{
				args = &args[++index];
				index = 0;
			}
			else
			{
				for (index++; args[index]; index++)
					free(args[index]);
				return (ret);
			}
		}
	}
	args = replace_aliases(args);
	ret = run_args(args, front, exe_ret);
	return (ret);
}

/**
 * get_args - This will get a command from standard input.
 * @line: serves as a buffer to store the command.
 * @exe_ret: return value of the last executed command.
 *
 * Return: If an error occurs - NULL.
 *         Else - a pointer to the stored command.
 */

char *get_args(char *line, int *exe_ret)
{
	size_t n = 0;
	ssize_t read;
	char *prompt = "$ ";

	if (line)
		free(line);

	read = _getline(&line, &n, STDIN_FILENO);
	if (read == -1)
		return (NULL);
	if (read == 1)
	{
		hist++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, prompt, 2);
		return (get_args(line, exe_ret));
	}

	line[read - 1] = '\0';
	variable_replacement(&line, exe_ret);
	handle_line(&line, read);

	return (line);
}
