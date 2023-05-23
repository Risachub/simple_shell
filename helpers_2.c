#include "shell.h"


void logical_ops(char *line, ssize_t *new_len);
void handle_line(char **line, ssize_t read);
ssize_t get_new_len(char *line);

/**
 * logical_ops - examines a line for logical operators "||" or "&&".
 * @line: pointer to the character to check in the line.
 * @new_len: Pointer to new_len in get_new_len function.
 */

void logical_ops(char *line, ssize_t *new_len)
{
	char prev, cur, nex;

	prev = *(line - 1);
	cur = *line;
	nex = *(line + 1);

	if (cur == '&')
	{
		if (nex == '&' && prev != ' ')
			(*new_len)++;
		else if (prev == '&' && nex != ' ')
			(*new_len)++;
	}
	else if (cur == '|')
	{
		if (nex == '|' && prev != ' ')
			(*new_len)++;
		else if (prev == '|' && nex != ' ')
			(*new_len)++;
	}
}


/**
 * handle_line - splitting a line read from standard input as needed.
 * @line: A pointer to a line read from standard input.
 * @read: The length of line.
 *
 * Description: Spaces are inserted to separate ";", "||", and "&&".
 *              Replaces "#" with '\0'.
 */

void handle_line(char **line, ssize_t read)
{
	char *old_line, *new_line;
	char prev, cur, nex;
	size_t i, j;
	ssize_t new_len;

	new_len = get_new_len(*line);
	if (new_len == read - 1)
		return;
	new_line = malloc(new_len + 1);
	if (!new_line)
		return;
	j = 0;
	old_line = *line;
	for (i = 0; old_line[i]; i++)
	{
		cur = old_line[i];
		nex = old_line[i + 1];
		if (i != 0)
		{
			prev = old_line[i - 1];
			if (cur == ';')
			{
				if (nex == ';' && prev != ' ' && prev != ';')
				{
					new_line[j++] = ' ';
					new_line[j++] = ';';
					continue;
				}
				else if (prev == ';' && nex != ' ')
				{
					new_line[j++] = ';';
					new_line[j++] = ' ';
					continue;
				}
				if (prev != ' ')
					new_line[j++] = ' ';
				new_line[j++] = ';';
				if (nex != ' ')
					new_line[j++] = ' ';
				continue;
			}
			else if (cur == '&')
			{
				if (nex == '&' && prev != ' ')
					new_line[j++] = ' ';
				else if (prev == '&' && nex != ' ')
				{
					new_line[j++] = '&';
					new_line[j++] = ' ';
					continue;
				}
			}
			else if (cur == '|')
			{
				if (nex == '|' && prev != ' ')
					new_line[j++]  = ' ';
				else if (prev == '|' && nex != ' ')
				{
					new_line[j++] = '|';
					new_line[j++] = ' ';
					continue;
				}
			}
		}
		else if (cur == ';')
		{
			if (i != 0 && old_line[i - 1] != ' ')
				new_line[j++] = ' ';
			new_line[j++] = ';';
			if (nex != ' ' && nex != ';')
				new_line[j++] = ' ';
			continue;
		}
		new_line[j++] = old_line[i];
	}
	new_line[j] = '\0';

	free(*line);
	*line = new_line;
}

/**
 * get_new_len - Finds the new length of a line separatored
 *               by ";", "||", "&&&", or "#".
 * @line: The line to check.
 *
 * Return: The new length of the line.
 *
 * Description: Splits short lines containing '#' comments with '\0'.
 */

ssize_t get_new_len(char *line)
{
	size_t index;
	ssize_t new_len = 0;
	char cur, nex;

	for (index = 0; line[index]; index++)
	{
		cur = line[index];
		nex = line[index + 1];
		if (cur == '#')
		{
			if (index == 0 || line[index - 1] == ' ')
			{
				line[index] = '\0';
				break;
			}
		}
		else if (index != 0)
		{
			if (cur == ';')
			{
				if (nex == ';' && line[index - 1] != ' ' && line[index - 1] != ';')
				{
					new_len += 2;
					continue;
				}
				else if (line[index - 1] == ';' && nex != ' ')
				{
					new_len += 2;
					continue;
				}
				if (line[index - 1] != ' ')
					new_len++;
				if (nex != ' ')
					new_len++;
			}
			else
				logical_ops(&line[index], &new_len);
		}
		else if (cur == ';')
		{
			if (index != 0 && line[index - 1] != ' ')
				new_len++;
			if (nex != ' ' && nex != ';')
				new_len++;
		}
		new_len++;
	}
	return (new_len);
}
