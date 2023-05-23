#include "shell.h"

void print_alias(alias_t *alias);
int shellby_alias(char **args, char __attribute__((__unused__)) **front);
void set_alias(char *var_name, char *value);

/**
 * print_alias - This prints the alias in the format name='value'.
 * @alias: Pointer to an alias.
 */

void print_alias(alias_t *alias)
{
	char *alias_string;
	int length = _strlen(alias->name) + _strlen(alias->value) + 4;

	alias_string = malloc(sizeof(char) * (length + 1));
	if (!alias_string)
		return;
	_strcpy(alias_string, alias->name);
	_strcat(alias_string, "='");
	_strcat(alias_string, alias->value);
	_strcat(alias_string, "'\n");

	write(STDOUT_FILENO, alias_string, length);
	free(alias_string);
}


/**
 * shellby_alias - A builtin command that either prints all aliases, specific
 * aliases, or sets an alias.
 * @args: array of arguments.
 * @front: double pointer to the beginning of args.
 *
 * Return: If an error occurs - -1.
 *        Else - 0.
 */

int shellby_alias(char **args, char __attribute__((__unused__)) **front)
{
	alias_t *temp = aliases;
	int index, ret = 0;
	char *value;

	if (!args[0])
	{
		while (temp)
		{
			print_alias(temp);
			temp = temp->next;
		}
		return (ret);
	}
	for (index = 0; args[index]; index++)
	{
		temp = aliases;
		value = _strchr(args[index], '=');
		if (!value)
		{
			while (temp)
			{
				if (_strcmp(args[i], temp->name) == 0)
				{
					print_alias(temp);
					break;
				}
				temp = temp->next;
			}
			if (!temp)
				ret = create_error(args + index, 1);
		}
		else
			set_alias(args[index], value);
	}
	return (ret);
}


/**
 * set_alias - This will either set an existing alias 'name' with a new value,
 * 'value' or creates a new alias with 'name' and 'value'.
 * @var_name: Name of the alias.
 * @value: Value of the alias. First character is a '='.
 */

void set_alias(char *var_name, char *value)
{
	alias_t *temp = aliases;
	int length, j, k;
	char *new_value;

	*value = '\0';
	value++;
	length = _strlen(value) - _strspn(value, "'\"");
	new_value = malloc(sizeof(char) * (length + 1));
	if (!new_value)
		return;
	for (j = 0, k = 0; value[j]; j++)
	{
		if (value[j] != '\'' && value[j] != '"')
			new_value[k++] = value[j];
	}
	new_value[k] = '\0';
	while (temp)
	{
		if (_strcmp(var_name, temp->name) == 0)
		{
			free(temp->value);
			temp->value = new_value;
			break;
		}
		temp = temp->next;
	}
	if (!temp)
		add_alias_end(&aliases, var_name, new_value);
}
