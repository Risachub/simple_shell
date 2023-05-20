#include "shell.h"

list_t *add_node_end(list_t **head, char *dir);
void free_list(list_t *head);
alias_t *add_alias_end(alias_t **head, char *name, char *value);
void free_alias_list(alias_t *head);

/**
 * add_alias_end - This adds a node to end of alias_t linked list.
 * @head: pointer to head of list_t list.
 * @name: name of new alias to be added.
 * @value: value of new alias to be added.
 *
 * Return: If an error occurs - NULL.
 *         Otherwise - a pointer to the new node.
 */

alias_t *add_alias_end(alias_t **head, char *name, char *value)
{
	alias_t *last;
	alias_t *new_node = malloc(sizeof(alias_t));

	if (!new_node)
	{
		return (NULL);
	}

	new_node->next = NULL;
	new_node->name = malloc(sizeof(char) * (_strlen(name) + 1));

	if (!new_node->name)
	{
		free(new_node);
		return (NULL);
	}
	new_node->value = value;
	_strcpy(new_node->name, name);

	if (*head)
	{
		last = *head;
		while (last->next != NULL)
			last = last->next;
		last->next = new_node;
	}
	else
		*head = new_node;

	return (new_node);
}

/**
 * add_node_end - Add node to end of list_t linked list.
 * @head: pointer to head of list_t list.
 * @dir: directory path for new node to contain.
 *
 * Return: If an error occurs - NULL.
 *         Otherwise - a pointer to the new node.
 */

list_t *add_node_end(list_t **head, char *dir)
{
	list_t *new_node = malloc(sizeof(list_t));
	list_t *last;


	if (!new_node)
	{
		return (NULL);
	}

	new_node->dir = dir;
	new_node->next = NULL;

	if (*head)
	{
		last = *head;
		while (last->next != NULL)
			last = last->next;
		last->next = new_node;
	}
	else
		*head = new_node;

	return (new_node);
}


/**
 * free_list - Frees list_t linked list.
 * @head: head of list_t list.
 */
void free_list(list_t *head)
{
	list_t *next;

	while (head)
	{
		next = head->next;
		free(head->dir);
		free(head);
		head = next;
	}
}


/**
 * free_alias_list - Frees alias_t linked list.
 * @head: head of alias_t list.
 */
void free_alias_list(alias_t *head)
{
	alias_t *next;

	while (head)
	{
		next = head->next;
		free(head->name);
		free(head->value);
		free(head);
		head = next;
	}
}
