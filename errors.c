#include "shell.h"

int create_error(char **args, int err);
int num_len(int num);
char *_itoa(int num);


/**
 * create_error - This will write a custom error message to stderr.
 * @args: array of arguments.
 * @err: error value.
 *
 * Return: error value.
 */

int create_error(char **args, int err)
{

	char *error;

	switch (err)
	{
	case -1:
		error = error_env(args);
		break;
	case 1:
		error = error_1(args);
		break;
	case 2:
		if (*(args[0]) == 'e')
			error = error_2_exit(++args);
		else if (args[0][0] == ';' || args[0][0] == '&' || args[0][0] == '|')
			error = error_2_syntax(args);
		else
			error = error_2_cd(args);
		break;
	case 126:
		error = error_126(args);
		break;
	case 127:
		error = error_127(args);
		break;
	}
	write(STDERR_FILENO, error, _strlen(error));

	if (error)
		free(error);
	return (err);

}

/**
 * num_len - This will count the digit length of a number.
 * @num: This is the number to measure.
 *
 * Return: The digit length.
 */


int num_len(int num)
{
	unsigned int num1;
	int len = 1;

	if (num < 0)
	{
		len++;
		num1 = num * -1;
	}
	else
	{
		num1 = num;
	}
	while (num1 > 9)
	{
		len++;
		num1 /= 10;
	}

	return (len);
}

/**
 * _itoa - This will convert an integer to a string.
 * @num: integer.
 *
 * Return: converted string.
 */

char *_itoa(int num)
{

	unsigned int num1;
	int len = num_len(num);
	char *buffer;


	buffer = malloc(sizeof(char) * (len + 1));
	if (!buffer)
		return (NULL);

	buffer[len] = '\0';

	if (num < 0)
	{
		num1 = num * -1;
		buffer[0] = '-';
	}
	else
	{
		num1 = num;
	}

	len--;
	do {
		buffer[len] = (num1 % 10) + '0';
		num1 /= 10;
		len--;
	} while (num1 > 0);

	return (buffer);
}
