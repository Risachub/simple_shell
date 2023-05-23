#include "shell.h"

int shellby_help(char **args, char __attribute__((__unused__)) **front);
int shellby_exit(char **args, char **front);
int (*get_builtin(char *command))(char **args, char **front);
int shellby_cd(char **args, char __attribute__((__unused__)) **front);

/**
 * shellby_help - shows information about shellby builtin commands.
 * @args: Is an array of arguments.
 * @front: Is a pointer to the beginning of args.
 *
 * Return: If an error occurs - -1.
 *         else - 0.
 */

int shellby_help(char **args, char __attribute__((__unused__)) **front)
{
	if (!args[0])
		help_all();
	else if (_strcmp(args[0], "alias") == 0)
		help_alias();
	else if (_strcmp(args[0], "cd") == 0)
		help_cd();
	else if (_strcmp(args[0], "exit") == 0)
		help_exit();
	else if (_strcmp(args[0], "env") == 0)
		help_env();
	else if (_strcmp(args[0], "setenv") == 0)
		help_setenv();
	else if (_strcmp(args[0], "unsetenv") == 0)
		help_unsetenv();
	else if (_strcmp(args[0], "help") == 0)
		help_help();
	else
		write(STDERR_FILENO, name, _strlen(name));

	return (0);
}


/**
 * shellby_exit - Terminates the shellby shell process.
 * @args: An array of arguments, including the exit value.
 * @front: A double pointer to the start of args.
 * Return: If no arguments are provided - Returns -3.
 * If the given exit value is invalid - Returns 2.
 * Otherwise, exits the shellby shell with the specified status value.
 * if no arguments are provided, the function returns -3, indicating that
 * exit value is provided, the function returns 2. Otherwise, the shellby shell
 * is exited with the specified status value.
 */

int shellby_exit(char **args, char **front)
{
	int j, len_of_int = 10;
	unsigned int num = 0, max = 1 << (sizeof(int) * 8 - 1);

	if (args[0])
	{
		if (args[0][0] == '+')
		{
			j = 1;
			len_of_int++;
		}
		for (; args[0][j]; j++)
		{
			if (j <= len_of_int && args[0][j] >= '0' && args[0][j] <= '9')
				num = (num * 10) + (args[0][j] - '0');
			else
				return (create_error(--args, 2));
		}
	}
	else
	{
		return (-3);
	}
	if (num > max - 1)
		return (create_error(--args, 2));
	args -= 1;
	free_args(args, front);
	free_env();
	free_alias_list(aliases);
	exit(num);
}



/**
 * get_builtin - This matches a command with a corresponding
 *               shellby builtin function.
 * @command: command to match.
 *
 * Return: function pointer to the corresponding builtin.
 */

int (*get_builtin(char *command))(char **args, char **front)
{
	builtin_t funcs[] = {
		{ "exit", shellby_exit },
		{ "env", shellby_env },
		{ "setenv", shellby_setenv },
		{ "unsetenv", shellby_unsetenv },
		{ "cd", shellby_cd },
		{ "alias", shellby_alias },
		{ "help", shellby_help },
		{ NULL, NULL }
	};
	int j;

	for (j = 0; funcs[j].name; j++)
	{
		if (_strcmp(funcs[j].name, command) == 0)
			break;
	}
	return (funcs[j].f);
}


/**
 * shellby_cd - This changes the current directory of the shellby process.
 * @args: Is an array of arguments.
 * @front: Is a double pointer to the beginning of args.
 *
 * Return: If the given string is not a directory - 2.
 *         If an error occurs - -1.
 *         Else - 0.
 */


int shellby_cd(char **args, char __attribute__((__unused__)) **front)
{
	char **directory_info, *new_line = "\n";
	char *oldpwd = NULL, *pwd = NULL;
	struct stat dir;

	oldpwd = getcwd(oldpwd, 0);
	if (!oldpwd)
		return (-1);

	if (args[0])
	{
		if (*(args[0]) == '-' || _strcmp(args[0], "--") == 0)
		{
			if ((args[0][1] == '-' && args[0][2] == '\0') ||
					args[0][1] == '\0')
			{
				if (_getenv("OLDPWD") != NULL)
					(chdir(*_getenv("OLDPWD") + 7));
			}
			else
			{
				free(oldpwd);
				return (create_error(args, 2));
			}
		}
		else
		{
			if (stat(args[0], &dir) == 0 && S_ISDIR(dir.st_mode)
					&& ((dir.st_mode & S_IXUSR) != 0))
				chdir(args[0]);
			else
			{
				free(oldpwd);
				return (create_error(args, 2));
			}
		}
	}
	else
	{
		if (_getenv("HOME") != NULL)
			chdir(*(_getenv("HOME")) + 5);
	}

	pwd = getcwd(pwd, 0);
	if (!pwd)
		return (-1);

	directory_info = malloc(sizeof(char *) * 2);
	if (!directory_info)
		return (-1);

	directory_info[0] = "OLDPWD";
	directory_info[1] = oldpwd;
	if (shellby_setenv(directory_info, directory_info) == -1)
		return (-1);

	directory_info[0] = "PWD";
	directory_info[1] = pwd;
	if (shellby_setenv(directory_info, directory_info) == -1)
		return (-1);
	if (args[0] && args[0][0] == '-' && args[0][1] != '-')
	{
		write(STDOUT_FILENO, pwd, _strlen(pwd));
		write(STDOUT_FILENO, new_line, 1);
	}
	free(oldpwd);
	free(pwd);
	free(directory_info);
	return (0);
}
