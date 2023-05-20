#include "shell.h"

void variable_replacement(char **args, int *exe_ret);
void free_args(char **args, char **front);
char *get_env_value(char *beginning, int len);
char *get_pid(void);

/**
 * variable_replacement - This will  Handle the variable replacement.
 * @line: This is a double pointer containing the command and arguments.
 * @exe_ret: This is a pointer to the return value
 * of the last executed command.
 *
 * Description: Replaces $$ with the current PID, $? with the return value
 *              of the last executed program, and envrionmental variables
 *              preceded by $ with their corresponding value.
 */


void variable_replacement(char **line, int *exe_ret)
{
	char *replacement = NULL, *old_line = NULL, *new_line;
	int j, k = 0, len;

	old_line = *line;

	for (j = 0; old_line[j]; j++)
	{
		if (old_line[j] == '$' && old_line[j + 1] &&
				old_line[j + 1] != ' ')
		{
			if (old_line[j + 1] == '$')
			{
				replacement = get_pid();
				k = j + 2;
			}
			else if (old_line[j + 1] == '?')
			{
				replacement = _itoa(*exe_ret);
				k = j + 2;
			}
			else if (old_line[j + 1])
			{
				/* extract the variable name to search for */
				for (k = j + 1; old_line[k] &&
						old_line[k] != '$' &&
						old_line[k] != ' '; k++)
					;
				len = k - (j + 1);
				replacement = get_env_value(&old_line[j + 1], len);
			}
			new_line = malloc(j + _strlen(replacement)
					  + _strlen(&old_line[k]) + 1);
			if (!line)
				return;
			new_line[0] = '\0';
			_strncat(new_line, old_line, j);
			if (replacement)
			{
				_strcat(new_line, replacement);
				free(replacement);
				replacement = NULL;
			}
			_strcat(new_line, &old_line[k]);
			free(old_line);
			*line = new_line;
			old_line = new_line;
			j = -1;
		}
	}
}


/**
 * free_args - This will Free up memory that's taken by args.
 * @args: This is a  null-terminated double pointer
 * containing commands/arguments.
 * @front: double pointer to the beginning of args.
 */


void free_args(char **args, char **front)
{
	size_t i;

	for (i = 0; args[i] || args[i + 1]; i++)
		free(args[i]);

	free(front);
}


/**
 * get_env_value - This will Get the value corresponding
 * to an environmental variable.
 * @beginning: this is the environmental variable to search for.
 * @len: This is the length of the environmental variable to search for.
 *
 * Return: If the variable is not found - an empty string.
 *         Else - the value of the environmental variable.
 *
 * Description: Our variables will be stored in the format VARIABLE=VALUE.
 */


char *get_env_value(char *beginning, int len)
{

	char **var_addr;
	char *replacement = NULL, *temp, *var;
	var = malloc(len + 1);


	var = malloc(len + 1);
	if (!var)
		return (NULL);
	var[0] = '\0';
	_strncat(var, beginning, len);

	var_addr = _getenv(var);
	free(var);
	if (var_addr)
	{
		temp = *var_addr;
		while (*temp != '=')
			temp++;
		temp++;
		replacement = malloc(_strlen(temp) + 1);
		if (replacement)
			_strcpy(replacement, temp);
	}

	return (replacement);
}


/**
 * get_pid - This will get the current process ID.
 * Description: This opens stat file, a space-delimited file containing
 *              information about the current process. The PID is the
 *              first word in the file. The function reads the PID into
 *              a buffer and replace the space at the end with a \0 byte.
 *
 * Return: The current process ID or NULL on failure.
 */

char *get_pid(void)
{

	ssize_t file;
	size_t i = 0;
	char *buffer;

	file = open("/proc/self/stat", O_RDONLY);
	if (file == -1)
	{
		perror("Cant read file");
		return (NULL);
	}
	buffer = malloc(120);
	if (!buffer)
	{
		close(file);
		return (NULL);
	}
	read(file, buffer, 120);
	while (buffer[i] != ' ')
		i++;
	buffer[i] = '\0';

	close(file);
	return (buffer);
}
