#include "shell.h"


int shellby_setenv(char **args, char __attribute__((__unused__)) **front);
int shellby_env(char **args, char __attribute__((__unused__)) **front);
int shellby_unsetenv(char **args, char __attribute__((__unused__)) **front);


/**
 * shellby_setenv - This changes/adds an environmental variable to the PATH.
 * @args: This is an array of arguments passed to the shell.
 * @front: This is a double pointer to the beginning of args.
 * Description: args[1] is the name of the new or existing PATH variable.
 *              args[2] is the value to set the new or changed variable to.
 *
 * Return: If an error occurs - -1.
 *         Else - 0.
 */

int shellby_setenv(char **args, char __attribute__((__unused__)) **front)
{
	char **env_var = NULL, **new_environ, *new_value;
	size_t size;
	int index;

	if (!args[0] || !args[1])
		return (create_error(args, -1));

	new_value = malloc(_strlen(args[0]) + 1 + _strlen(args[1]) + 1);
	if (!new_value)
		return (create_error(args, -1));
	_strcpy(new_value, args[0]);
	_strcat(new_value, "=");
	_strcat(new_value, args[1]);

	env_var = _getenv(args[0]);
	if (env_var)
	{
		free(*env_var);
		*env_var = new_value;
		return (0);
	}
	for (size = 0; environ[size]; size++)
		;

	new_environ = malloc(sizeof(char *) * (size + 2));
	if (!new_environ)
	{
		free(new_value);
		return (create_error(args, -1));
	}

	for (index = 0; environ[index]; index++)
		new_environ[index] = environ[index];

	free(environ);
	environ = new_environ;
	environ[index] = new_value;
	environ[index + 1] = NULL;

	return (0);
}



/**
 * shellby_env - This prints the current environment.
 * @args: array of arguments passed to the shell.
 * @front: double pointer to the beginning of args.
 *
 * Return: If an error occurs - -1.
 *	  Else - 0.
 *
 * Description: This function prints the current environment variables
 * displaying one variable per line in the format 'variable'='value'.
 *   It provides an overview of the environment configuration in use.
 */

int shellby_env(char **args, char __attribute__((__unused__)) **front)
{

	char nc = '\n';
	int i;

	if (!environ)
		return (-1);

	for (i = 0; environ[i]; i++)
	{
		write(STDOUT_FILENO, environ[i], _strlen(environ[i]));
		write(STDOUT_FILENO, &nc, 1);
	}

	(void)args;
	return (0);
}


/**
 * shellby_unsetenv - This deletes an environmental variable from the PATH.
 * @args: array of arguments passed to the shell.
 * @front: double pointer to the beginning of args.
 * Description: args[1] is the PATH variable to remove.
 *
 * Return: If an error occurs - -1.
 *         Else - 0.
 */

int shellby_unsetenv(char **args, char __attribute__((__unused__)) **front)
{
	char **env_var, **new_environ;
	size_t size;
	int i, index2;

	if (!args[0])
		return (create_error(args, -1));
	env_var = _getenv(args[0]);
	if (!env_var)
		return (0);

	for (size = 0; environ[size]; size++)
		;

	new_environ = malloc(sizeof(char *) * size);
	if (!new_environ)
		return (create_error(args, -1));

	for (i = 0, index2 = 0; environ[i]; i++)
	{
		if (*env_var == environ[i])
		{
			free(*env_var);
			continue;
		}
		new_environ[index2] = environ[i];
		index2++;
	}
	free(environ);
	environ = new_environ;
	environ[size - 1] = NULL;

	return (0);
}
